//AUTOGENERATED BY genForallN.py
  
#ifndef __DOMAIN_FORALL2_H__
#define __DOMAIN_FORALL2_H__

#include<RAJA/RAJA.hxx>
#include<Domain/Tile.h>



/******************************************************************
 *  Policy base class, forall2()
 ******************************************************************/

    // Execute (Termination default)
    struct Forall2_Execute_Tag {};
    struct Forall2_Execute {
      typedef Forall2_Execute_Tag PolicyTag;
    };

    // Starting (outer) policy for all forall2 policies
    template<typename POL_I=RAJA::seq_exec, typename POL_J=RAJA::seq_exec, typename NEXT=Forall2_Execute>
    struct Forall2_Policy {
      typedef NEXT NextPolicy;
      typedef POL_I PolicyI;
      typedef POL_J PolicyJ;
    };

    // Interchange loop order given permutation
    struct Forall2_Permute_Tag {};
    template<typename LOOP_ORDER, typename NEXT=Forall2_Execute>
    struct Forall2_Permute {
      typedef Forall2_Permute_Tag PolicyTag;
      typedef NEXT NextPolicy;
      typedef LOOP_ORDER LoopOrder;
    };

    // Begin OpenMP Parallel Region
    struct Forall2_OMP_Parallel_Tag {};
    template<typename NEXT=Forall2_Execute>
    struct Forall2_OMP_Parallel {
      typedef Forall2_OMP_Parallel_Tag PolicyTag;
      typedef NEXT NextPolicy;
    };

    // Tiling Policy
    struct Forall2_Tile_Tag {};
    template<typename TILE_I, typename TILE_J, typename NEXT=Forall2_Execute>
    struct Forall2_Tile {
      typedef Forall2_Tile_Tag PolicyTag;
      typedef NEXT NextPolicy;
      typedef TILE_I TileI;
      typedef TILE_J TileJ;
    };


/******************************************************************
 *  OpenMP Auto-Collapsing Executors for forall2()
 ******************************************************************/

#ifdef _OPENMP

    // OpenMP Executor with collapse(2) for omp_parallel_for_exec
    template<>
    class Forall2Executor<RAJA::omp_parallel_for_exec, RAJA::omp_parallel_for_exec, RAJA::RangeSegment, RAJA::RangeSegment> {
      public:  
        template<typename BODY>
        inline void operator()(RAJA::RangeSegment const &is_i, RAJA::RangeSegment const &is_j, BODY body) const {
          int const i_start = is_i.getBegin();
          int const i_end   = is_i.getEnd();

          int const j_start = is_j.getBegin();
          int const j_end   = is_j.getEnd();

#pragma omp parallel for schedule(static) collapse(2)
          for(int i = i_start;i < i_end;++ i){
            for(int j = j_start;j < j_end;++ j){
              body(i, j);
          } } 
        }
    };

    // OpenMP Executor with collapse(2) for omp_for_nowait_exec
    template<>
    class Forall2Executor<RAJA::omp_for_nowait_exec, RAJA::omp_for_nowait_exec, RAJA::RangeSegment, RAJA::RangeSegment> {
      public:  
        template<typename BODY>
        inline void operator()(RAJA::RangeSegment const &is_i, RAJA::RangeSegment const &is_j, BODY body) const {
          int const i_start = is_i.getBegin();
          int const i_end   = is_i.getEnd();

          int const j_start = is_j.getBegin();
          int const j_end   = is_j.getEnd();

#pragma omp for schedule(static) collapse(2) nowait
          for(int i = i_start;i < i_end;++ i){
            for(int j = j_start;j < j_end;++ j){
              body(i, j);
          } } 
        }
    };


#endif // _OPENMP


/******************************************************************
 *  forall2_policy() Foreward declarations
 ******************************************************************/

    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_policy(Forall2_Execute_Tag, TI const &is_i, TJ const &is_j, BODY body);

    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_policy(Forall2_Permute_Tag, TI const &is_i, TJ const &is_j, BODY body);

    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_policy(Forall2_OMP_Parallel_Tag, TI const &is_i, TJ const &is_j, BODY body);

    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_policy(Forall2_Tile_Tag, TI const &is_i, TJ const &is_j, BODY body);


/******************************************************************
 *  Forall2Executor(): Default Executor for loops
 ******************************************************************/

    template<typename POLICY_I, typename POLICY_J, typename TI, typename TJ>
    struct Forall2Executor {
      template<typename BODY>
      inline void operator()(TI const &is_i, TJ const &is_j, BODY body) const {
        RAJA::forall<POLICY_I>(is_i, RAJA_LAMBDA(int i){
          RAJA::forall<POLICY_J>(is_j, RAJA_LAMBDA(int j){
            body(i, j);
          });
        });
      }
    };


/******************************************************************
 *  forall2_permute(): Permutation function overloads
 ******************************************************************/

    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_permute(PERM_IJ, TI const &is_i, TJ const &is_j, BODY body){
      typedef typename POLICY::NextPolicy            NextPolicy;
      typedef typename POLICY::NextPolicy::PolicyTag NextPolicyTag;

      // Call next policy with permuted indices and policies
      forall2_policy<NextPolicy, PolicyI, PolicyJ>(NextPolicyTag(), is_i, is_j,
        RAJA_LAMBDA(int i, int j){
          // Call body with non-permuted indices
          body(i, j);
        });
    }

    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_permute(PERM_JI, TI const &is_i, TJ const &is_j, BODY body){
      typedef typename POLICY::NextPolicy            NextPolicy;
      typedef typename POLICY::NextPolicy::PolicyTag NextPolicyTag;

      // Call next policy with permuted indices and policies
      forall2_policy<NextPolicy, PolicyJ, PolicyI>(NextPolicyTag(), is_j, is_i,
        RAJA_LAMBDA(int j, int i){
          // Call body with non-permuted indices
          body(i, j);
        });
    }


/******************************************************************
 *  forall2_policy() Policy Layer, overloads for policy tags
 ******************************************************************/


    /**
     * Execute inner loops policy function.
     * This is the default termination case.
     */
    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_policy(Forall2_Execute_Tag, TI const &is_i, TJ const &is_j, BODY body){

      // Create executor object to launch loops
      Forall2Executor<PolicyI, PolicyJ, TI, TJ> exec;

      // Launch loop body
      exec(is_i, is_j, body);
    }


    /**
     * Permutation policy function.
     * Provides loop interchange.
     */
    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_policy(Forall2_Permute_Tag, TI const &is_i, TJ const &is_j, BODY body){
      // Get the loop permutation
      typedef typename POLICY::LoopOrder LoopOrder;

      // Call loop interchange overload to re-wrire indicies and policies
      forall2_permute<POLICY, PolicyI, PolicyJ>(LoopOrder(), is_i, is_j, body);
    }


    /**
     * OpenMP Parallel Region Section policy function.
     */
    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_policy(Forall2_OMP_Parallel_Tag, TI const &is_i, TJ const &is_j, BODY body){
      typedef typename POLICY::NextPolicy            NextPolicy;
      typedef typename POLICY::NextPolicy::PolicyTag NextPolicyTag;

      // create OpenMP Parallel Region
#ifdef _OPENMP
#pragma omp parallel
#endif
      {
        // execute the next policy
        forall2_policy<NextPolicy, PolicyI, PolicyJ>(NextPolicyTag(), is_i, is_j, body);
      }
    }


    /**
     * Tiling policy function.
     */
    template<typename POLICY, typename PolicyI, typename PolicyJ, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2_policy(Forall2_Tile_Tag, TI const &is_i, TJ const &is_j, BODY body){
      typedef typename POLICY::NextPolicy            NextPolicy;
      typedef typename POLICY::NextPolicy::PolicyTag NextPolicyTag;
      typedef typename POLICY::TileI TileI;
      typedef typename POLICY::TileJ TileJ;

      // execute the next policy
      forall_tile(TileI(), is_i, [=](RAJA::RangeSegment is_ii){
        forall_tile(TileJ(), is_j, [=](RAJA::RangeSegment is_jj){
          forall2_policy<NextPolicy, PolicyI, PolicyJ>(NextPolicyTag(), is_ii, is_jj, body);
        });
      });
    }



/******************************************************************
 * forall2(), User interface
 * Provides index typing, and initial nested policy unwrapping
 ******************************************************************/

    template<typename POLICY, typename IdxI=int, typename IdxJ=int, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2(TI const &is_i, TJ const &is_j, BODY body){
      // extract next policy
      typedef typename POLICY::NextPolicy             NextPolicy;
      typedef typename POLICY::NextPolicy::PolicyTag  NextPolicyTag;

      // extract each loop's execution policy
      typedef typename POLICY::PolicyI                PolicyI;
      typedef typename POLICY::PolicyJ                PolicyJ;

      // call 'policy' layer with next policy
      forall2_policy<NextPolicy, PolicyI, PolicyJ>(NextPolicyTag(), is_i, is_j, 
        [=](int i, int j){
          body(IdxI(i), IdxJ(j));
        }
      );
    }


  
#endif

