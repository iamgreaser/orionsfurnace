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
#include <poll.h>
#include <unistd.h>

#if 1
const int NETWORK_SOCKTYPE = SOCK_STREAM;
const int NETWORK_PROTOCOL = IPPROTO_TCP;
#else
// TODO: No really, abolish Windows
const int NETWORK_SOCKTYPE = SOCK_SEQPACKET;
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
}

void TCPPipeEnd::pump_send(void)
{
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
