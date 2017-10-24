
#ifndef __NODEPOOL_HPP__
#define __NODEPOOL_HPP__
#include <list>
#include "common.hpp"
#include "Node.hpp"
#include "ArrayPool.hpp"
#include "Function.hpp"

class NodePool {
  typedef std::list<Node*> NodeList;
  NodeList m_list;

public:

  //get an object from the pool
  NodePtr get() {
    
    Node *p = NULL;
    if(m_list.size() > 0) {
      p = m_list.back();
      m_list.pop_back();
    }else{
      p = new Node();
    }
    p->set_id(NodePool::global_id());
    
    return NodePtr(p, [](Node* np) {
	NodePool::global()->dispose(np);
      });
  }

  template<class T>
  NodePtr get_seq_scalar(MPI_Comm comm, T val) {
    NodePtr p = NodePool::global()->get();
    ArrayPtr ap = oa::funcs::consts(comm, {1, 1, 1}, val, 0);
    p->set_type(TYPE_DATA);
    p->set_data(ap);
    p->set_scalar();
    p->set_seqs();
    return p;
  }
  
  // throw the object into object pool 
  void dispose(Node* n) {
    n -> reset();
    m_list.push_back(n);
  }

  static NodePool* global() {
    static NodePool np;
    return &np;
  }

  static int global_id() {
    static int m_global_id = 0;
    m_global_id += 1;
    return m_global_id;
  }

};

#endif