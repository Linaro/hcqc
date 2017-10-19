typedef int local_int_t;
typedef long long global_int_t;

struct Geometry_STRUCT {
  int size; //!< Number of MPI processes
  int rank; //!< This process' rank in the range [0 to size - 1]
  int numThreads; //!< This process' number of threads
  int nx;   //!< Number of x-direction grid points for each local subdomain
  int ny;   //!< Number of y-direction grid points for each local subdomain
  int nz;   //!< Number of z-direction grid points for each local subdomain
  int npx;  //!< Number of processors in x-direction
  int npy;  //!< Number of processors in y-direction
  int npz;  //!< Number of processors in z-direction
  int ipx;  //!< Current rank's x location in the npx by npy by npz processor grid
  int ipy;  //!< Current rank's y location in the npx by npy by npz processor grid
  int ipz;  //!< Current rank's z location in the npx by npy by npz processor grid

};
typedef struct Geometry_STRUCT Geometry;

struct Vector_STRUCT {
  local_int_t localLength;  //!< length of local portion of the vector
  double * values;          //!< array of values
  /*!
   This is for storing optimized data structures created in OptimizeProblem and
   used inside optimized ComputeSPMV().
   */
  void * optimizationData;

};
typedef struct Vector_STRUCT Vector;

struct SparseMatrix_STRUCT {
  char  * title; //!< name of the sparse matrix
  Geometry * geom; //!< geometry associated with this matrix
  global_int_t totalNumberOfRows; //!< total number of matrix rows across all processes
  global_int_t totalNumberOfNonzeros; //!< total number of matrix nonzeros across all processes
  local_int_t localNumberOfRows; //!< number of rows local to this process
  local_int_t localNumberOfColumns;  //!< number of columns local to this process
  local_int_t localNumberOfNonzeros;  //!< number of nonzeros local to this process
  char  * nonzerosInRow;  //!< The number of nonzeros in a row will always be 27 or fewer
  global_int_t ** mtxIndG; //!< matrix indices as global values
  local_int_t ** mtxIndL; //!< matrix indices as local values
  double ** matrixValues; //!< values of matrix entries
  double ** matrixDiagonal; //!< values of matrix diagonal entries
#if 0
  std::map< global_int_t, local_int_t > globalToLocalMap; //!< global-to-local mapping
  std::vector< global_int_t > localToGlobalMap; //!< local-to-global mapping
  mutable bool isDotProductOptimized;
  mutable bool isSpmvOptimized;
  mutable bool isMgOptimized;
  mutable bool isWaxpbyOptimized;
  /*!
   This is for storing optimized data structres created in OptimizeProblem and
   used inside optimized ComputeSPMV().
   */
  mutable struct SparseMatrix_STRUCT * Ac; // Coarse grid matrix
  mutable MGData * mgData; // Pointer to the coarse level data for this fine matrix
#endif
  void * optimizationData;  // pointer that can be used to store implementation-specific data
};
typedef struct SparseMatrix_STRUCT SparseMatrix;
