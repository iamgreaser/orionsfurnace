#include "net/net.h"

using net::Node;

Node::Node(std::istream &ips, std::ostream &ops)
	: m_ips(&ips)
	, m_ops(&ops)
{
}

Node::~Node(void)
{
}
