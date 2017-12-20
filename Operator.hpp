#ifndef __OPERATOR_HPP__
#define __OPERATOR_HPP__ 

#include "NodePool.hpp"
#include "NodeDesc.hpp"

#define LIB_KERNEL_PATH "./libkernel.so"

namespace oa {
  namespace ops {

    template<class T>
    NodePtr new_seqs_scalar_node(MPI_Comm comm, T val){
      return(NodePool::global()->get_seqs_scalar(comm, val));
    }
    
    NodePtr new_node(const ArrayPtr &ap);

    NodePtr new_node(NodeType type, NodePtr u, NodePtr v);

    NodePtr new_node(NodeType type, NodePtr u);

    const NodeDesc& get_node_desc(NodeType type);

    void write_graph(const NodePtr& root, bool is_root = true,
                     const char *filename = "graph.dot");

    ArrayPtr eval(NodePtr A);

    ArrayPtr force_eval(NodePtr A);

    void get_kernel_parameter(NodePtr A, vector<void*> &list, 
      PartitionPtr &ptr);
    
    const KernelPtr get_kernel_dict(size_t hash, 
                                    const char *filename = "fusion-kernels");

    void insert_kernel_dict(size_t hash, const stringstream &s,
                            const char *filename = "fusion-kernels");

    void gen_kernels(NodePtr A, bool is_root = true, MPI_Comm = MPI_COMM_WORLD);

    void gen_kernels_JIT(NodePtr A, 
        bool is_root = true, MPI_Comm comm = MPI_COMM_WORLD);

    void gen_kernels_JIT_with_op(NodePtr A, 
        bool is_root = true, MPI_Comm comm = MPI_COMM_WORLD);

    void tree_to_string(NodePtr A, stringstream &ss);

    void tree_to_code(NodePtr A, stringstream &ss, int &id);

    void tree_to_code(NodePtr A, stringstream &ss, int &id,
      vector<int>& int_id, vector<int>& float_id, vector<int>& double_id);

    void tree_to_code_with_op(NodePtr A, stringstream &ss, int &id, int &S_id,
      vector<int>& int_id, vector<int>& float_id, vector<int>& double_id);

    void change_string_with_op(stringstream& ss, string in, const NodeDesc &nd);

    string replace_string(string& in, const string& old_str, const string& new_str);

    void tree_to_string_stack(NodePtr A, stringstream &ss);
  }
}


#endif
