#include "Node.hpp"
#include "NodeDesc.hpp"
#include "Operator.hpp"

using namespace std;

Node::Node() {}

Node::Node(NodePtr u) {}

//Node(ArrayPtr array);
Node::~Node() {}

void Node::display(char const *prefix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  if (rank == 0) {
    printf("Node %s, id = %d\n", prefix, id);

    const NodeDesc& nd = oa::ops::get_node_desc(m_type);
    printf("Node type is %s\n", nd.name.c_str());
  }

  if (m_type == TYPE_DATA) {
    if (!m_is_seqs) m_data->display(prefix);
    else if (!rank) m_data->display(prefix);
  }
}

int Node::input_size() const {
  return m_input.size();
}

NodePtr Node::input(int i) {
  return m_input.at(i);
}

NodePtr Node::output(int i) {
  return m_output.at(i);
}

void Node::add_input(int pos, NodePtr in) {
  m_input.insert(m_input.begin() + pos, in);
}

void Node::add_output(int pos, NodePtr out) {
  m_output.push_back(out);
}

void Node::set_type(NodeType type) {
  m_type = type;
}

void Node::set_data(const ArrayPtr& ptr) {
  m_data = ptr;
}

void Node::set_id(int _id) {
  id = _id;
}

int Node::get_id() const {
  return id;
}

size_t Node::hash() {
  return m_hash;
}

NodeType Node::type() {
  return m_type;
}

ArrayPtr Node::get_data() {
  return m_data;
}

bool Node::has_data() {
  return m_data != NULL;
}

void Node::reset() {
  
}

bool Node::is_scalar() const {
  return m_is_scalar;
}

bool Node::is_seqs() const {
  return m_is_seqs;
}

void Node::set_scalar() {
  m_is_scalar = true;
  m_data->set_scalar();
}

void Node::set_seqs() {
  m_is_seqs = true;
  m_data->set_seqs();
}

bool Node::is_seqs_scalar() const {
  return m_is_seqs && m_is_scalar;
}
