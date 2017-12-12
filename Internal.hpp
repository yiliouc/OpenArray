#ifndef __INTERNAL_HPP__
#define __INTERNAL_HPP__

#include <random>
#include "common.hpp"
#include "Box.hpp"
#include "utils/utils.hpp"
#include <vector>

extern "C"{
  void tic(const char* s);
  void toc(const char* s);
  void show_time(const char* s);
  void show_all();
}

namespace oa {
  namespace internal {

    int calc_id(int i, int j, int k, int3 S);

    template <typename T>
    void set_buffer_consts(T *buffer, int size, T val) {
      for (int i = 0; i < size; i++) buffer[i] = val;
    }

    //sub_A = sub(A, box)
    template <typename T>
    void get_buffer_subarray(T *sub_buffer, T *buffer, const Box &sub_box,
      const Box &box, int sw) {
      
      Shape sp = box.shape(sw);
      int M = sp[0];
      int N = sp[1];
      int P = sp[2];
			
      Box bd_box = box.boundary_box(sw);
      Box ref_box = sub_box.ref_box(bd_box);
      int xs, xe, ys, ye, zs, ze;
      ref_box.get_corners(xs, xe, ys, ye, zs, ze, sw);
      
      //ref_box.display("ref_box");

      int cnt = 0;
      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            sub_buffer[cnt++] = buffer[k * M * N + j * M + i];
            //cout<<buffer[cnt-1]<<" ";
          }
          //cout<<endl;
        }
        //cout<<endl;
      }
    }

    // set sub(A) = B
    template<typename T1, typename T2>
    void set_buffer_subarray(T1* buffer, T2* sub_buffer, const Box &box,
      const Box &sub_box, int sw) {

      Shape sp = box.shape(sw);
      int M = sp[0];
      int N = sp[1];
      int P = sp[2];
      
      Box bd_box = box.boundary_box(sw);
      Box ref_box = sub_box.ref_box(bd_box);
      int xs, xe, ys, ye, zs, ze;
      ref_box.get_corners(xs, xe, ys, ye, zs, ze, sw);
      
      //ref_box.display("ref_box");

      int cnt = 0;
      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            if (zs + sw <= k && k < ze - sw &&
                ys + sw <= j && j < ye - sw &&
                xs + sw <= i && i < xe - sw) {
              buffer[k * M * N + j * M + i] = sub_buffer[cnt];
            }
            cnt++;
            //cout<<buffer[cnt-1]<<" ";
          }
          //cout<<endl;
        }
        //cout<<endl;
      }
    }

    // set sub(A) = const
    template<typename T1, typename T2>
    void set_buffer_subarray_const(T1* buffer, T2 val, const Box &box, 
      const Box &sub_box, int sw) {

      Shape sp = box.shape(sw);
      int M = sp[0];
      int N = sp[1];
      int P = sp[2];
      
      Box bd_box = box.boundary_box(sw);
      Box ref_box = sub_box.ref_box(bd_box);
      int xs, xe, ys, ye, zs, ze;
      ref_box.get_corners(xs, xe, ys, ye, zs, ze, sw);
      
      //ref_box.display("ref_box");

      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            if (zs + sw <= k && k < ze - sw &&
                ys + sw <= j && j < ye - sw &&
                xs + sw <= i && i < xe - sw) {
              buffer[k * M * N + j * M + i] = val;
            }
            //cout<<buffer[cnt-1]<<" ";
          }
          //cout<<endl;
        }
        //cout<<endl;
      }
    }



    template<typename T>
    void set_buffer_rand(T *buffer, int size) {
      arma::Col<T> v = oa::utils::make_vec(size, buffer);
      v.randu();
    }

    //specialized for int type
    template<>
    void set_buffer_rand(int *buffer, int size);

    
    template<typename T>
    void set_buffer_seqs(T *buffer, const Shape& s, Box box, int sw) {
      int cnt = 0;
      int xs, xe, ys, ye, zs, ze;
      int M = s[0];
      int N = s[1];
      int P = s[2];
      //cout<<M<<" "<<N<<" "<<P<<endl;
      box.get_corners(xs, xe, ys, ye, zs, ze, sw);
      //printf("%d %d %d %d %d %d\n", xs, xe, ys, ye, zs, ze);
      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            buffer[cnt++] = k * M * N + j * M + i;
            //cout<<buffer[cnt-1]<<" ";
          }
          //cout<<endl;
        }
        //cout<<endl;
      }
    }

    template <typename T>
    void set_ghost_consts(T *buffer, const Shape &sp, T val, int sw = 1) {
      int M = sp[0] + 2 * sw;
      int N = sp[1] + 2 * sw;
      int P = sp[2] + 2 * sw;

      int cnt = 0;
      for (int k = 0; k < P; k++) {
        for (int j = 0; j < N; j++) {
          for (int i = 0; i < M; i++) {
            if ((sw <= k && k < P - sw) &&
                (sw <= j && j < N - sw) &&
                (sw <= i && i < M - sw)) {
              cnt++;
              continue;
            }
            buffer[cnt++] = val;
          }
        }
      }
    }


    int3 calc_step(Box box, int d, int sw);

    ///:mute
    ///:include "NodeType.fypp"
    ///:endmute
    ///:for k in [i for i in L if (i[3] == 'A' or i[3] == 'B' or i[3] == 'F')]
    ///:set name = k[1]
    ///:set sy = k[2]
    // A = B ${sy}$ val
    template<typename T1, typename T2, typename T3>
    void buffer_${name}$_const(T1 *A, T2 *B, T3 val, int size) {
      for (int i = 0; i < size; i++) {
        A[i] = B[i] ${sy}$ val;
      }
    }

    template<typename T1, typename T2, typename T3>
    void const_${name}$_buffer(T1 *A, T2 val, T3 *B, int size) {
      for (int i = 0; i < size; i++) {
        A[i] = val ${sy}$ B[i];
      }
    }

    // A = U ${sy}$ V
    template<typename T1, typename T2, typename T3>
    void buffer_${name}$_buffer(T1 *A, T2 *U, T3 *V, int size) {

      // arma::Col<T1> CA = oa::utils::make_vec<T1>(size, A);
      // arma::Col<T2> CU = oa::utils::make_vec<T2>(size, U);
      // arma::Col<T3> CV = oa::utils::make_vec<T3>(size, V);

      ///:set a = sy
      ///:if a == '*'
      ///:set a = '%'
      ///:endif
       // CA = CU ${a}$ CV;
      
      //std::cout<<CA(arma::span(1, 10))<<std::endl;
      
      //tic("kernel");
      
#pragma omp parallel for
      for (int i = 0; i < size; i++) {
        A[i] = U[i] ${sy}$ V[i];
      }
      //toc("kernel");
    }

    // A = U pesudo ${sy}$ V
    template<typename T1, typename T2, typename T3>
    void pseudo_buffer_${name}$_buffer(T1 *A, T2 *U, T3 *V, 
      Box ab, Box ub, Box vb, Shape Sa, Shape Su, Shape Sv, int sw) {

      int3 a_x = calc_step(ab, 0, sw);
      int3 a_y = calc_step(ab, 1, sw);
      int3 a_z = calc_step(ab, 2, sw);

      int3 u_x = calc_step(ub, 0, sw);
      int3 u_y = calc_step(ub, 1, sw);
      int3 u_z = calc_step(ub, 2, sw);

      int3 v_x = calc_step(vb, 0, sw);
      int3 v_y = calc_step(vb, 1, sw);
      int3 v_z = calc_step(vb, 2, sw);

      // oa::utils::mpi_order_start(MPI_COMM_WORLD);
      
      // printf("a\n");
      // printf("%d %d %d\n", a_x[0], a_x[1], a_x[2]);
      // printf("%d %d %d\n", a_y[0], a_y[1], a_y[2]);
      // printf("%d %d %d\n", a_z[0], a_z[1], a_z[2]);

      // printf("u\n");
      // printf("%d %d %d\n", u_x[0], u_x[1], u_x[2]);
      // printf("%d %d %d\n", u_y[0], u_y[1], u_y[2]);
      // printf("%d %d %d\n", u_z[0], u_z[1], u_z[2]);

      // printf("v\n");
      // printf("%d %d %d\n", v_x[0], v_x[1], v_x[2]);
      // printf("%d %d %d\n", v_y[0], v_y[1], v_y[2]);
      // printf("%d %d %d\n", v_z[0], v_z[1], v_z[2]);
      
      // oa::utils::mpi_order_end(MPI_COMM_WORLD);

      for (int ka = a_z[0], ku = u_z[0], kv = v_z[0]; 
        ka < a_z[2] && ku < u_z[2] && kv < v_z[2];
        ka += a_z[1], ku += u_z[1], kv += v_z[1]) {
        
        for (int ja = a_y[0], ju = u_y[0], jv = v_y[0];
          ja < a_y[2] && ju < u_y[2] && jv < v_y[2];
          ja += a_y[1], ju += u_y[1], jv += v_y[1]) {

          for (int ia = a_x[0], iu = u_x[0], iv = v_x[0];
            ia < a_x[2] && iu < u_x[2] && iv < v_x[2];
            ia += a_x[1], iu += u_x[1], iv += v_x[1]) {

            // printf("%d %d %d %d %d %d %d\n", ia, ja, ka, Sa[0], Sa[1], Sa[2], 
            //   calc_id(ia, ja, ka, Sa));
            A[calc_id(ia, ja, ka, Sa)] = 
              U[calc_id(iu, ju, ku, Su)] ${sy}$ V[calc_id(iv, jv, kv, Sv)];
          }
        }
      }

    }

    ///:endfor 

    // A = B
    template<typename T>
    void copy_buffer(T *A, T *B, int size) {
      for (int i = 0; i < size; i++) {
        A[i] = B[i];
      }
    }

    ///:mute
    ///:include "NodeType.fypp"
    ///:endmute
    ///:for k in [i for i in L if (i[3] == 'C')]
    ///:set name = k[1]
    ///:set sy = k[2]
    ///:set ef = k[7]
    // ans = ${ef}$
    template<typename T1, typename T2>
    void buffer_${name}$(T1 *A, T2 *B, int size) {
      for (int i = 0; i < size; i++) {
        ///:if name != 'rcp'
        A[i] = ${sy}$(B[i]);
        ///:else
        A[i] = 1.0 / B[i];
        ///:endif
      }
    }

    ///:endfor

    template<typename T>
    void buffer_pow(double *A, T *B, double m, int size) {
      for (int i = 0; i < size; i++) {
        A[i] = pow(B[i], m);
      }
    }

    template<typename T>
    void buffer_not(int *A, T *B, int size) {
      for (int i = 0; i < size; i++) {
        A[i] = !(B[i]);
      }
    }

    ///:for k in [['max','>',''],['min','<',''],['abs_max','>'],['abs_min','<']]
    ///:set name = k[0]
    ///:set sy = k[1]
    template<typename T>
    void buffer_${name}$_const(T &val, int* pos, T *A, Box box, int sw) {
      int x = 0, y = 0, z = 0;
      int xs, xe, ys, ye, zs, ze;
      box.get_corners(xs, xe, ys, ye, zs, ze, sw);

      int M = xe - xs;
      int N = ye - ys;
      int K = ze - zs;

      //std::cout<<"MNK:"<<M<<" "<<N<<" "<<" "<<K<<" sw="<<sw<<std::endl;
      
      ///:mute
      ///:if k[0:3] == 'abs'
      ///:set op = "abs"
      ///:else
      ///:set op = ""
      ///:endif
      ///:endmute
      
      val = ${op}$(A[sw * M * N + sw * M + sw]);

      pos[0] = xs;
      pos[1] = ys;
      pos[2] = zs;

      //oa::utils::mpi_order_start(MPI_COMM_WORLD);
      for (int k = sw; k < K-sw; k++) {
        for (int j = sw; j < N-sw; j++) {
          for (int i = sw; i < M-sw; i++) {
            //printf("(%d,%d,%d) = %d"
            // std::cout<<"("<<i<<","<<j<<","<<k<<")="
            //          <<A[i + j * M + k * M * N]<<std::endl;
            
            if (A[i + j * M + k * M * N] ${sy}$ ${op}$(val)) {
              val = A[i + j * M + k * M * N];
              pos[0] = i - sw + xs;
              pos[1] = j - sw + ys;
              pos[2] = k - sw + zs;
            }
          }
        }
      }
      //oa::utils::mpi_order_end(MPI_COMM_WORLD);
    }
    ///:endfor

    template<typename T>
    void buffer_sum_scalar_const(T *val, T *A, Box box, int sw, int size) {
      int x = 0, y = 0, z = 0;
      int xs, xe, ys, ye, zs, ze;
      box.get_corners(xs, xe, ys, ye, zs, ze, sw);

      int M = xe - xs;
      int N = ye - ys;
      int K = ze - zs;
      *val = 0;

      int cnt = 0;
      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            if ((xs + sw <= i && i < xe - sw) &&
                (ys + sw <= j && j < ye - sw) &&
                (zs + sw <= k && k < ze - sw)) {
              *val += A[cnt];
            }
            cnt++;
          }
        }
      }
    }

    template<typename T>
    void buffer_csum_x_const(T *ap, T *A, Box box, int sw, int size, T *buffer, int type) {
    //type:   top 2  mid 1  bottom 0
      int x = 0, y = 0, z = 0;
      int xs, xe, ys, ye, zs, ze;
      box.get_corners(xs, xe, ys, ye, zs, ze, sw);

      int M = xe - xs;
      int N = ye - ys;
      int K = ze - zs;

      int cnt = 0;
      int dcnt = 0;
      if(type == 2) 
        for(int i = 0; i < (ye-ys-2*sw)*(ze-zs-2*sw); i++)
          buffer[i] = 0;

      int index = 0;
      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            if ((xs + sw <= i && i < xe - sw) &&
                (ys + sw <= j && j < ye - sw) &&
                (zs + sw <= k && k < ze - sw)) {
              int temp1 = (xe-xs)*(ye-ys)*(k-zs)+(xe-xs)*(j-ys)+(i-xs);
              if((i == xs + sw) && (type == 1 || type == 0))
                ap[temp1] = buffer[index++];
              else 
                ap[temp1] = 0;
            }
          }
        }
      }

      index = 0;
      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            if ((xs + sw <= i && i < xe - sw) &&
                (ys + sw <= j && j < ye - sw) &&
                (zs + sw <= k && k < ze - sw)) {
              int temp1 = (xe-xs)*(ye-ys)*(k-zs)+(xe-xs)*(j-ys)+(i-xs);
              int temp2 = (xe-xs)*(ye-ys)*(k-zs)+(xe-xs)*(j-ys)+(i-xs+1);
              ap[temp1]+=A[temp1];
              if(i < xe - sw - 1){
                ap[temp2] += ap[temp1];
              }
              if((i == xe - sw - 1) && (type == 1 || type == 2))
                buffer[index++] = ap[temp1];
            }
          }
        }
      }
    }


    template<typename T>
    void buffer_csum_y_const(T *ap, T *A, Box box, int sw, int size, T *buffer, int type) {
    //type:   top 2  mid 1  bottom 0
      int x = 0, y = 0, z = 0;
      int xs, xe, ys, ye, zs, ze;
      box.get_corners(xs, xe, ys, ye, zs, ze, sw);

      int M = xe - xs;
      int N = ye - ys;
      int K = ze - zs;

      int cnt = 0;
      int dcnt = 0;
      if(type == 2) 
        for(int i = 0; i < (xe-xs-2*sw)*(ze-zs-2*sw); i++)
          buffer[i] = 0;

      int index = 0;
      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            if ((xs + sw <= i && i < xe - sw) &&
                (ys + sw <= j && j < ye - sw) &&
                (zs + sw <= k && k < ze - sw)) {
              int temp1 = (xe-xs)*(ye-ys)*(k-zs)+(xe-xs)*(j-ys)+(i-xs);
              if((j == ys + sw) && (type == 1 || type == 0))
                ap[temp1] = buffer[index++];
              else 
                ap[temp1] = 0;
            }
          }
        }
      }
      index = 0;
      for (int k = zs; k < ze; k++) {
        for (int i = xs; i < xe; i++) {
          for (int j = ys; j < ye; j++) {
            if ((xs + sw <= i && i < xe - sw) &&
                (ys + sw <= j && j < ye - sw) &&
                (zs + sw <= k && k < ze - sw)) {
              int temp1 = (xe-xs)*(ye-ys)*(k-zs)+(xe-xs)*(j-ys)+(i-xs);
              int temp2 = (xe-xs)*(ye-ys)*(k-zs)+(xe-xs)*(j+1-ys)+(i-xs);
              ap[temp1]+=A[temp1];
              if(j < ye - sw -1){
                ap[temp2] += ap[temp1];
              }
              if((j == ye - sw -1) && (type == 1 || type == 2))
                buffer[index++] = ap[temp1];
            }
          }
        }
      }
    }

    template<typename T>
    void buffer_csum_z_const(T *ap, T *A, Box box, int sw, int size, T *buffer, int type) {
    //type:   top 2  mid 1  bottom 0
      int x = 0, y = 0, z = 0;
      int xs, xe, ys, ye, zs, ze;
      box.get_corners(xs, xe, ys, ye, zs, ze, sw);

      int M = xe - xs;
      int N = ye - ys;
      int K = ze - zs;

      int cnt = 0;
      int dcnt = 0;
      if(type == 2) 
        for(int i = 0; i < (ye-ys-2*sw)*(xe-xs-2*sw); i++)
          buffer[i] = 0;

      int index = 0;
      for (int k = zs; k < ze; k++) {
        for (int j = ys; j < ye; j++) {
          for (int i = xs; i < xe; i++) {
            if ((xs + sw <= i && i < xe - sw) &&
                (ys + sw <= j && j < ye - sw) &&
                (zs + sw <= k && k < ze - sw)) {
              int temp1 = (xe-xs)*(ye-ys)*(k-zs)+(xe-xs)*(j-ys)+(i-xs);
              if((k == zs + sw) && (type == 1 || type == 0))
                ap[temp1] = buffer[index++];
              else 
                ap[temp1] = 0;
            }
          }
        }
      }
      index = 0;
      for (int j = ys; j < ye; j++) {
        for (int i = xs; i < xe; i++) {
          for (int k = zs; k < ze; k++) {
            if ((xs + sw <= i && i < xe - sw) &&
                (ys + sw <= j && j < ye - sw) &&
                (zs + sw <= k && k < ze - sw)) {
              int temp1 = (xe-xs)*(ye-ys)*(k-zs)+(xe-xs)*(j-ys)+(i-xs);
              int temp2 = (xe-xs)*(ye-ys)*(k+1-zs)+(xe-xs)*(j-ys)+(i-xs);
              ap[temp1]+=A[temp1];
              if(k < ze - sw - 1){
                ap[temp2] += ap[temp1];
              }
              if((k == ze - sw - 1) && (type == 1 || type == 2))
                buffer[index++] = ap[temp1];
            }
          }
        }
      }
    }

    // template<typename T>
    // inline int calc_id(int i, int j, int k, int3 S) {
    //   int M = S[0];
    //   int N = S[1];
    //   int P = S[2];
    //   return k * M * N + j * M + i;
    // }


    ///:mute
    ///:include "NodeType.fypp"
    ///:endmute
    ///:for k in [i for i in L if i[3] == 'D']
    ///:set name = k[1]
    
    ///:set func = ''
    ///:if name == "axb"
    ///:set func = '0.5 * (b[calc_id(i, j, k, S)] + b[calc_id(i - 1, j, k, S)])'
    ///:elif name == "axf"
    ///:set func = '0.5 * (b[calc_id(i, j, k, S)] + b[calc_id(i + 1, j, k, S)])'
    ///:elif name == "ayb"
    ///:set func = '0.5 * (b[calc_id(i, j, k, S)] + b[calc_id(i, j - 1, k, S)])'
    ///:elif name == "ayf"
    ///:set func = '0.5 * (b[calc_id(i, j, k, S)] + b[calc_id(i, j + 1, k, S)])'
    ///:elif name == "azb"
    ///:set func = '0.5 * (b[calc_id(i, j, k, S)] + b[calc_id(i, j, k - 1, S)])'
    ///:elif name == "azf"
    ///:set func = '0.5 * (b[calc_id(i, j, k, S)] + b[calc_id(i, j, k + 1, S)])'
    ///:elif name == "dxb"
    ///:set func = '1.0 * (b[calc_id(i, j, k, S)] - b[calc_id(i - 1, j, k, S)]) / 1.0'
    ///:elif name == "dxf"
    ///:set func = '1.0 * (-b[calc_id(i, j, k, S)] + b[calc_id(i + 1, j, k, S)]) / 1.0'
    ///:elif name == "dyb"
    ///:set func = '1.0 * (b[calc_id(i, j, k, S)] - b[calc_id(i, j - 1, k, S)]) / 1.0'
    ///:elif name == "dyf"
    ///:set func = '1.0 * (-b[calc_id(i, j, k, S)] + b[calc_id(i, j + 1, k, S)]) / 1.0'
    ///:elif name == "dzb"
    ///:set func = '1.0 * (b[calc_id(i, j, k, S)] - b[calc_id(i, j, k - 1, S)]) / 1.0'
    ///:elif name == "dzf"
    ///:set func = '1.0 * (-b[calc_id(i, j, k, S)] + b[calc_id(i, j, k + 1, S)]) / 1.0'
    ///:endif

    // crate kernel_${name}$
    // A = ${name}$(U)
    template<typename T>
    void ${name}$_calc_inside(double* ans, T* b, int3 lbound, int3 rbound, 
      int sw, Shape sp, Shape S) {

      for (int k = sw + lbound[2]; k < sw + sp[2] - rbound[2]; k++) {
        for (int j = sw + lbound[1]; j < sw + sp[1] - rbound[1]; j++) {
          for (int i = sw + lbound[0]; i < sw + sp[0] - rbound[0]; i++) {
            
            ans[calc_id(i, j, k, S)] = ${func}$;
            
          }
        }
      }
    }

    template<typename T>
    void ${name}$_calc_outside(double* ans, T* b, int3 lbound, int3 rbound, 
      int sw, Shape sp, Shape S) {
      int M = S[0];
      int N = S[1];
      int P = S[2];

      // update outside one surface (contains boundary, doesn't care)

      ///:if name[1:] == 'zb'
      for (int k = sw; k < sw + lbound[2]; k++) {
        for (int j = 0; j < N; j++) {
          for (int i = 0; i < M; i++) {
            ans[calc_id(i, j, k, S)] = ${func}$;
          }
        }
      }

      ///:elif name[1:] == 'zf'
      for (int k = sw + sp[2] - rbound[2]; k < sw + sp[2]; k++) {
        for (int j = 0; j < N; j++) {
          for (int i = 0; i < M; i++) {
            ans[calc_id(i, j, k, S)] = ${func}$;
          }
        }
      }

      ///:elif name[1:] == 'yb'
      for (int k = 0; k < P; k++) {
        for (int j = sw; j < sw + lbound[1]; j++) {
          for (int i = 0; i < M; i++) {
            ans[calc_id(i, j, k, S)] = ${func}$;
          }
        }
      }

      ///:elif name[1:] == 'yf'
      for (int k = 0; k < P; k++) {
        for (int j = sw + sp[1] - rbound[1]; j < sw + sp[1]; j++) {
          for (int i = 0; i < M; i++) {
            ans[calc_id(i, j, k, S)] = ${func}$;
          }
        }
      }

      ///:elif name[1:] == 'xb'
      for (int k = 0; k < P; k++) {
        for (int j = 0; j < N; j++) {
          for (int i = sw; i < sw + lbound[0]; i++) {
            ans[calc_id(i, j, k, S)] = ${func}$;
          }
        }
      }

      ///:elif name[1:] == 'xf'
      for (int k = 0; k < P; k++) {
        for (int j = 0; j < N; j++) {
          for (int i = sw + sp[0] - rbound[0]; i < sw + sp[0]; i++) {
            ans[calc_id(i, j, k, S)] = ${func}$;
          }
        }
      }

      ///:endif

    }

    ///:endfor

  }
}

#endif
