/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2011  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FUNCTION_PARAM_STORE_H
#define __FUNCTION_PARAM_STORE_H

#include <OSDefines.h>
#include <MiscUtils.h>

// This class is only needed on some platforms
#if defined(OS_ARCH_x64) && defined(GLI_BUILD_WINDOWS) 

//@
//  Summary:
//    This class provides a way to store function parameters for built in functions
//    - in the same way as extension loaded functions. 
//
struct FunctionParamStore
{
  //@
  //  Summary:
  //    To convert the passed parameter to a 64 bit type.
  //
  template <class A>
  inline uintptr_t ConvertParam(A a_a)
  {
    CASSERT(sizeof(A) <= sizeof(uintptr_t), __Invlaid_Type__);

    // Copy the raw bits of the type into a uintptr_t type
    // (could do pointer type cast, but using a union to look cleaner)
    union 
    {
      uintptr_t m_ptrType;
      A m_valueType;
    } retValue;

    //retValue.m_ptrType = 0; //Need to init?

    retValue.m_valueType = a_a;
    return retValue.m_ptrType;
  }

  //@
  //  Summary:
  //    To construct the parameter storage with the specified number of arguments
  //
  FunctionParamStore()
  {
    m_paramStore[0] = m_paramStore[1] = 0;
  }

  template <class A>
  FunctionParamStore(A a_a)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
  }

  template <class A, class B>
  FunctionParamStore(A a_a, B a_b)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
  }

  template <class A, class B, class C>
  FunctionParamStore(A a_a, B a_b, C a_c)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
  }

  template <class A, class B, class C, class D>
  FunctionParamStore(A a_a, B a_b, C a_c, D a_d)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
    m_paramStore[6] = m_paramStore[7] = ConvertParam(a_d);
  }

  template <class A, class B, class C, class D, class E>
  FunctionParamStore(A a_a, B a_b, C a_c, D a_d, E a_e)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
    m_paramStore[6] = m_paramStore[7] = ConvertParam(a_d);
    m_paramStore[8] = ConvertParam(a_e);
  }

  template <class A, class B, class C, class D, class E, class F>
  FunctionParamStore(A a_a, B a_b, C a_c, D a_d, E a_e, F a_f)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
    m_paramStore[6] = m_paramStore[7] = ConvertParam(a_d);

    m_paramStore[8] = ConvertParam(a_e);
    m_paramStore[9] = ConvertParam(a_f);
  }

  template <class A, class B, class C, class D, class E, class F, class G>
  FunctionParamStore(A a_a, B a_b, C a_c, D a_d, E a_e, F a_f, G a_g)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
    m_paramStore[6] = m_paramStore[7] = ConvertParam(a_d);

    m_paramStore[8] = ConvertParam(a_e);
    m_paramStore[9] = ConvertParam(a_f);
    m_paramStore[10] = ConvertParam(a_g);
  }

  template <class A, class B, class C, class D, class E, class F, class G, class H>
  FunctionParamStore(A a_a, B a_b, C a_c, D a_d, E a_e, F a_f, G a_g, H a_h)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
    m_paramStore[6] = m_paramStore[7] = ConvertParam(a_d);

    m_paramStore[8] = ConvertParam(a_e);
    m_paramStore[9] = ConvertParam(a_f);
    m_paramStore[10] = ConvertParam(a_g);
    m_paramStore[11] = ConvertParam(a_h);
  }

  template <class A, class B, class C, class D, class E, class F, class G, class H, class I>
  FunctionParamStore(A a_a, B a_b, C a_c, D a_d, E a_e, F a_f, G a_g, H a_h, I a_i)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
    m_paramStore[6] = m_paramStore[7] = ConvertParam(a_d);

    m_paramStore[8] = ConvertParam(a_e);
    m_paramStore[9] = ConvertParam(a_f);
    m_paramStore[10] = ConvertParam(a_g);
    m_paramStore[11] = ConvertParam(a_h);
    m_paramStore[12] = ConvertParam(a_i);
  }

  template <class A, class B, class C, class D, class E, class F, class G, class H, class I, class J>
  FunctionParamStore(A a_a, B a_b, C a_c, D a_d, E a_e, F a_f, G a_g, H a_h, I a_i, J a_j)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
    m_paramStore[6] = m_paramStore[7] = ConvertParam(a_d);

    m_paramStore[8] = ConvertParam(a_e);
    m_paramStore[9] = ConvertParam(a_f);
    m_paramStore[10] = ConvertParam(a_g);
    m_paramStore[11] = ConvertParam(a_h);
    m_paramStore[12] = ConvertParam(a_i);
    m_paramStore[13] = ConvertParam(a_j);
  }

  template <class A, class B, class C, class D, class E, class F, class G, class H, class I, class J, class K>
  FunctionParamStore(A a_a, B a_b, C a_c, D a_d, E a_e, F a_f, G a_g, H a_h, I a_i, J a_j, K a_k)
  {
    m_paramStore[0] = m_paramStore[1] = ConvertParam(a_a);
    m_paramStore[2] = m_paramStore[3] = ConvertParam(a_b);
    m_paramStore[4] = m_paramStore[5] = ConvertParam(a_c);
    m_paramStore[6] = m_paramStore[7] = ConvertParam(a_d);

    m_paramStore[8] = ConvertParam(a_e);
    m_paramStore[9] = ConvertParam(a_f);
    m_paramStore[10] = ConvertParam(a_g);
    m_paramStore[11] = ConvertParam(a_h);
    m_paramStore[12] = ConvertParam(a_i);
    m_paramStore[13] = ConvertParam(a_j);
    m_paramStore[14] = ConvertParam(a_k);
  }

  uintptr_t m_paramStore[20];  // An emulation of the parameter stack 

};


#endif // OS_ARCH_x64 && GLI_BUILD_WINDOWS

#endif // __FUNCTION_PARAM_STORE_H
