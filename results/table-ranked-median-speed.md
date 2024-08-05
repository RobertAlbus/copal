# Ranked by Median Speed

## [scalar, SIMD] [float, double] Benchmarks
 variant | type   | class             | method     | cpu time    
---------|--------|-------------------|------------|-------------
 simd    | float  | copal::VectorStdx | sin_stdlib | 836230 ns   
 simd    | float  | copal::VectorImpl | sin_taylor | 892691 ns   
 simd    | double | copal::VectorStdx | sin_stdlib | 2383336 ns  
 simd    | double | copal::VectorImpl | sin_taylor | 2449915 ns  
 simd    | float  | copal::VectorStdx | sin_taylor | 3781056 ns  
 simd    | float  | copal::VectorImpl | sin_lookup | 4899062 ns  
 simd    | double | copal::VectorStdx | sin_taylor | 5278337 ns  
 scalar  | float  | copal::Stdlib     | sin_stdlib | 7071179 ns  
 simd    | float  | copal::VectorStdx | sin_lookup | 10505258 ns 
 scalar  | float  | copal::Stdlib     | sin_taylor | 11490429 ns 
 scalar  | float  | copal::Scalar     | sin_taylor | 11627278 ns 
 scalar  | double | copal::Scalar     | sin_taylor | 11713989 ns 
 scalar  | double | copal::Stdlib     | sin_taylor | 11727096 ns 
 scalar  | double | copal::Stdlib     | sin_stdlib | 13975843 ns 
 simd    | double | copal::VectorImpl | sin_lookup | 15497064 ns 
 scalar  | float  | copal::Scalar     | sin_lookup | 19203047 ns 
 scalar  | float  | copal::Stdlib     | sin_lookup | 23552492 ns 
 simd    | double | copal::VectorStdx | sin_lookup | 25916659 ns 
 scalar  | double | copal::Scalar     | sin_lookup | 31460548 ns 
 scalar  | double | copal::Stdlib     | sin_lookup | 36165303 ns 


## [scalar, SIMD] float Benchmarks
 variant | type  | class             | method     | cpu time    
---------|-------|-------------------|------------|-------------
 simd    | float | copal::VectorStdx | sin_stdlib | 836230 ns   
 simd    | float | copal::VectorImpl | sin_taylor | 892691 ns   
 simd    | float | copal::VectorStdx | sin_taylor | 3781056 ns  
 simd    | float | copal::VectorImpl | sin_lookup | 4899062 ns  
 scalar  | float | copal::Stdlib     | sin_stdlib | 7071179 ns  
 simd    | float | copal::VectorStdx | sin_lookup | 10505258 ns 
 scalar  | float | copal::Stdlib     | sin_taylor | 11490429 ns 
 scalar  | float | copal::Scalar     | sin_taylor | 11627278 ns 
 scalar  | float | copal::Scalar     | sin_lookup | 19203047 ns 
 scalar  | float | copal::Stdlib     | sin_lookup | 23552492 ns 


## [scalar, SIMD] double Benchmarks
 variant | type   | class             | method     | cpu time    
---------|--------|-------------------|------------|-------------
 simd    | double | copal::VectorStdx | sin_stdlib | 2383336 ns  
 simd    | double | copal::VectorImpl | sin_taylor | 2449915 ns  
 simd    | double | copal::VectorStdx | sin_taylor | 5278337 ns  
 scalar  | double | copal::Scalar     | sin_taylor | 11713989 ns 
 scalar  | double | copal::Stdlib     | sin_taylor | 11727096 ns 
 scalar  | double | copal::Stdlib     | sin_stdlib | 13975843 ns 
 simd    | double | copal::VectorImpl | sin_lookup | 15497064 ns 
 simd    | double | copal::VectorStdx | sin_lookup | 25916659 ns 
 scalar  | double | copal::Scalar     | sin_lookup | 31460548 ns 
 scalar  | double | copal::Stdlib     | sin_lookup | 36165303 ns 


## scalar [float, double] Benchmarks
 variant | type   | class         | method     | cpu time    
---------|--------|---------------|------------|-------------
 scalar  | float  | copal::Stdlib | sin_stdlib | 7071179 ns  
 scalar  | float  | copal::Stdlib | sin_taylor | 11490429 ns 
 scalar  | float  | copal::Scalar | sin_taylor | 11627278 ns 
 scalar  | double | copal::Scalar | sin_taylor | 11713989 ns 
 scalar  | double | copal::Stdlib | sin_taylor | 11727096 ns 
 scalar  | double | copal::Stdlib | sin_stdlib | 13975843 ns 
 scalar  | float  | copal::Scalar | sin_lookup | 19203047 ns 
 scalar  | float  | copal::Stdlib | sin_lookup | 23552492 ns 
 scalar  | double | copal::Scalar | sin_lookup | 31460548 ns 
 scalar  | double | copal::Stdlib | sin_lookup | 36165303 ns 


## scalar float Benchmarks
 variant | type  | class         | method     | cpu time    
---------|-------|---------------|------------|-------------
 scalar  | float | copal::Stdlib | sin_stdlib | 7071179 ns  
 scalar  | float | copal::Stdlib | sin_taylor | 11490429 ns 
 scalar  | float | copal::Scalar | sin_taylor | 11627278 ns 
 scalar  | float | copal::Scalar | sin_lookup | 19203047 ns 
 scalar  | float | copal::Stdlib | sin_lookup | 23552492 ns 


## scalar double Benchmarks
 variant | type   | class         | method     | cpu time    
---------|--------|---------------|------------|-------------
 scalar  | double | copal::Scalar | sin_taylor | 11713989 ns 
 scalar  | double | copal::Stdlib | sin_taylor | 11727096 ns 
 scalar  | double | copal::Stdlib | sin_stdlib | 13975843 ns 
 scalar  | double | copal::Scalar | sin_lookup | 31460548 ns 
 scalar  | double | copal::Stdlib | sin_lookup | 36165303 ns 


## SIMD [float, double] Benchmarks
 variant | type   | class             | method     | cpu time    
---------|--------|-------------------|------------|-------------
 simd    | float  | copal::VectorStdx | sin_stdlib | 836230 ns   
 simd    | float  | copal::VectorImpl | sin_taylor | 892691 ns   
 simd    | double | copal::VectorStdx | sin_stdlib | 2383336 ns  
 simd    | double | copal::VectorImpl | sin_taylor | 2449915 ns  
 simd    | float  | copal::VectorStdx | sin_taylor | 3781056 ns  
 simd    | float  | copal::VectorImpl | sin_lookup | 4899062 ns  
 simd    | double | copal::VectorStdx | sin_taylor | 5278337 ns  
 simd    | float  | copal::VectorStdx | sin_lookup | 10505258 ns 
 simd    | double | copal::VectorImpl | sin_lookup | 15497064 ns 
 simd    | double | copal::VectorStdx | sin_lookup | 25916659 ns 


## SIMD float Benchmarks
 variant | type  | class             | method     | cpu time    
---------|-------|-------------------|------------|-------------
 simd    | float | copal::VectorStdx | sin_stdlib | 836230 ns   
 simd    | float | copal::VectorImpl | sin_taylor | 892691 ns   
 simd    | float | copal::VectorStdx | sin_taylor | 3781056 ns  
 simd    | float | copal::VectorImpl | sin_lookup | 4899062 ns  
 simd    | float | copal::VectorStdx | sin_lookup | 10505258 ns 


## SIMD double Benchmarks
 variant | type   | class             | method     | cpu time    
---------|--------|-------------------|------------|-------------
 simd    | double | copal::VectorStdx | sin_stdlib | 2383336 ns  
 simd    | double | copal::VectorImpl | sin_taylor | 2449915 ns  
 simd    | double | copal::VectorStdx | sin_taylor | 5278337 ns  
 simd    | double | copal::VectorImpl | sin_lookup | 15497064 ns 
 simd    | double | copal::VectorStdx | sin_lookup | 25916659 ns 
