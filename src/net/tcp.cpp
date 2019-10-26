/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "net/tcp.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

// TODO: Abolish Windows. But until then, this needs Winsock support.
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <unistd.h>

// Use 64KB for recv
const int TCP_RECV_BUFFER_SIZE = (64<<10);

#if 1
const int NETWORK_SOCKTYPE = SOCK_STREAM;
const int NETWORK_PROTOCOL = IPPROTO_TCP;
#else
// TODO: No really, abolish Windows
//const int NETWORK_SOCKTYPE = SOCK_SEQPACKET;
const int NETWORK_SOCKTYPE = SOCK_STREAM;
const int NETWORK_PROTOCOL = IPPROTO_SCTP;
#endif

using net::TCPPipeEnd;
using net::TCPServer;

TCPPipeEnd::TCPPipeEnd(int sockfd)
	: m_sockfd(sockfd)
{
}

TCPPipeEnd::TCPPipeEnd(std::string addr, int port)
{
	// Set up hints for GAI
	struct addrinfo hints = {};
	hints.ai_socktype = NETWORK_SOCKTYPE;
	hints.ai_protocol = NETWORK_PROTOCOL;

	// Run GAI
	struct addrinfo *gai = NULL;
	int did_gai = getaddrinfo(
		addr.c_str(),
		std::to_string(port).c_str(),
		&hints,
		&gai);

	if (gai == NULL) {
		std::cerr << "Could not look up host: \"" << std::string(gai_strerror(did_gai)) << std::endl;
		assert(!"Could not connect: Could not look up host");
	}

	// Grab the first thing we can get our hands on
	m_sockfd = socket(
		gai->ai_family,
		gai->ai_socktype,
		gai->ai_protocol);

	if (m_sockfd < 0) {
		perror("creating client socket");
		assert(!"Could not connect: Could not create socket");
	}

	std::cout << "Client address family: " << gai->ai_family << std::endl;
	std::cout << "Client socket type: " << gai->ai_socktype << std::endl;
	std::cout << "Client protocol: " << gai->ai_protocol << std::endl;

	// Disable Nagle's algorithm and send immediately
	int tcp_nodelay = 1;
	int did_tcp_nodelay = setsockopt(
		m_sockfd, IPPROTO_TCP, TCP_NODELAY,
		(void *)&tcp_nodelay,
		sizeof(tcp_nodelay));

	if (did_tcp_nodelay < 0) {
		perror("setting TCP_NODELAY");
		assert(!"Could not host: Could not set TCP_NODELAY");
	}


	// Now connect to it
	int did_connect = connect(
		m_sockfd,
		(const struct sockaddr *)gai->ai_addr,
		(socklen_t)gai->ai_addrlen);

	if (did_connect < 0) {
		perror("connecting to client socket");
		assert(!"Could not connect: Could not, well, connect");
	}

	// Free GAI
	freeaddrinfo(gai);
}

TCPPipeEnd::~TCPPipeEnd(void)
{
	if (m_sockfd != -1) {
		shutdown(m_sockfd, SHUT_RDWR);
		close(m_sockfd);
		m_sockfd = -1;
	}
}

void TCPPipeEnd::pump_recv(void)
{
	if (m_is_dead) {
		return;
	}

	char recv_buf[TCP_RECV_BUFFER_SIZE];
	ssize_t got_bytes = recv(
		m_sockfd,
		recv_buf,
		sizeof(recv_buf),
		MSG_DONTWAIT);

	if (got_bytes < 0) {
		int e = errno;

		// Check if this is because we would block.
		// NOTE: On Linux they're the same, but POSIX allows them to differ.
		if (e == EAGAIN) { return; }
		if (e == EWOULDBLOCK) { return; }

		// Mark this socket as dead
		perror("recv");
		m_is_dead = true;
		return;
		//assert(!"recv failed");
	}

	// Did we disconnect?
	if (got_bytes == 0) {
		// Mark this socket as dead
		std::cerr << "recv returned 0" << std::endl;
		m_is_dead = true;
		return;
		//assert(!"TODO: Disconnect elegantly");
	}

	// Add this to the buffer.
	m_recv_ss.write(recv_buf, got_bytes);
}

void TCPPipeEnd::pump_send(void)
{
	if (m_is_dead) {
		return;
	}

	// Fetch some output
	{
		char buf[8192];
		std::streamsize sz = m_send_ss.readsome(buf, 8192);
		m_send_buf += std::string(buf, sz);
	}

	// Don't send nothing.
	// - This can easily get confused with EOF.
	// - IIRC FreeBSD throws an error if you do this, but it's been years since I last checked.
	if (m_send_buf.size() == 0) {
		return;
	}

	// Write some bytes
	ssize_t wrote_bytes = send(
		m_sockfd,
		m_send_buf.c_str(),
		m_send_buf.size(),
		MSG_DONTWAIT);

	if (wrote_bytes < 0) {
		int e = errno;

		// Check if this is because we would block.
		// NOTE: On Linux they're the same, but POSIX allows them to differ.
		if (e == EAGAIN) { return; }
		if (e == EWOULDBLOCK) { return; }

		// Mark this socket as dead
		perror("send");
		m_is_dead = true;
		return;
		//assert(!"send failed");
	}

	// Did we disconnect?
	if (wrote_bytes == 0) {
		std::cerr << "send returned 0" << std::endl;
		m_is_dead = true;
		return;
		//assert(!"TODO: Disconnect elegantly");
	}

	// Remove this from the send buffer.
	m_send_buf.erase(m_send_buf.begin(), m_send_buf.begin()+wrote_bytes);
}

TCPServer::TCPServer(int port)
{
	// Set up hints for GAI
	struct addrinfo hints = {};
	hints.ai_socktype = NETWORK_SOCKTYPE;
	hints.ai_protocol = NETWORK_PROTOCOL;

	// Run GAI
	struct addrinfo *gai_base = NULL;
	int did_gai = getaddrinfo(
		std::string("localhost").c_str(),
		std::to_string(port).c_str(),
		&hints,
		&gai_base);


	struct addrinfo *gai = gai_base;

	// Find an IPv4 or IPv6 port
	while (gai != NULL && !this->is_good_ai_family(gai->ai_family)) {
		gai = gai->ai_next;
	}

	if (gai == NULL) {
		std::cerr << "Could not look up host: \"" << std::string(gai_strerror(did_gai)) << std::endl;
		assert(!"Could not host: Could not look up host");
	}

	std::cout << "Server address family: " << gai->ai_family << std::endl;
	std::cout << "Server socket type: " << gai->ai_socktype << std::endl;
	std::cout << "Server protocol: " << gai->ai_protocol << std::endl;

	// Grab the first thing we can get our hands on
	m_sockfd = socket(
		gai->ai_family,
		gai->ai_socktype,
		gai->ai_protocol);

	if (m_sockfd < 0) {
		perror("creating server socket");
		assert(!"Could not host: Could not create socket");
	}

	// Make sure it's easy to kick out something hogging the port
	int reuse_addr = 1;
	int did_reuse_addr = setsockopt(
		m_sockfd, SOL_SOCKET, SO_REUSEADDR,
		(void *)&reuse_addr,
		sizeof(reuse_addr));

	if (did_reuse_addr < 0) {
		perror("setting SO_REUSEADDR");
		assert(!"Could not host: Could not set SO_REUSEADDR");
	}

	// Now bind it
	int did_bind = bind(
		m_sockfd,
		(const struct sockaddr *)gai->ai_addr,
		(socklen_t)gai->ai_addrlen);

	if (did_bind < 0) {
		perror("binding to server socket");
		assert(!"Could not host: bind failed");
	}

	// Finally, start listening
	int did_listen = listen(m_sockfd, 2);

	if (did_listen < 0) {
		perror("listening to server socket");
		assert(!"Could not host: listen failed");
	}

	// Free GAI
	freeaddrinfo(gai_base);
}

TCPServer::~TCPServer(void)
{
	if (m_sockfd != -1) {
		shutdown(m_sockfd, SHUT_RDWR);
		close(m_sockfd);
		m_sockfd = -1;
	}
}

bool TCPServer::is_good_ai_family(int family)
{
	switch (family) {
		case AF_LOCAL:
			return false;

		case AF_INET:
		case AF_INET6:
			return true;

		default:
			return false;
	}
}

TCPPipeEnd *TCPServer::accept_if_available(void)
{
	// Check for new client connections
	struct pollfd base_fds[1] = {};
	base_fds[0].fd = m_sockfd;
	base_fds[0].events = POLLIN;
	base_fds[0].revents = 0;
	int poll_result = poll(base_fds, 1, 0);

	if (poll_result < 0) {
		perror("server root socket poll");
		assert(!"could not poll server root socket");
	}

	// Accept if we can
	if ((base_fds[0].revents & POLLIN) != 0) {
		// Accept!
		int new_sockfd = accept(m_sockfd, NULL, NULL);
		if (new_sockfd < 0) {
			perror("server root socket accept");
			assert(!"could not accept from server root socket");
		}

		// Now add a client
		net::TCPPipeEnd *pipe_end = new net::TCPPipeEnd(
			new_sockfd);

		// And return!
		return pipe_end;
	}

	// Nothing to return.
	return NULL;
}
