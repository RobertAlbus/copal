# Ranked by Median Speed

## [scalar, SIMD] [float, double] Benchmarks
 variant | type   | class             | method     | cpu time    
---------|--------|-------------------|------------|-------------
 simd    | float  | copal::VectorStdx | sin_stdlib | 833680 ns   
 simd    | float  | copal::VectorImpl | sin_taylor | 890045 ns   
 simd    | double | copal::VectorStdx | sin_stdlib | 2375847 ns  
 simd    | double | copal::VectorImpl | sin_taylor | 2442514 ns  
 simd    | float  | copal::VectorStdx | sin_taylor | 3769487 ns  
 simd    | float  | copal::VectorImpl | sin_lookup | 4883027 ns  
 simd    | double | copal::VectorStdx | sin_taylor | 5261707 ns  
 scalar  | float  | copal::Stdlib     | sin_stdlib | 7050784 ns  
 simd    | float  | copal::VectorStdx | sin_lookup | 10470986 ns 
 scalar  | float  | copal::Stdlib     | sin_taylor | 11460499 ns 
 scalar  | float  | copal::Scalar     | sin_taylor | 11594652 ns 
 scalar  | double | copal::Scalar     | sin_taylor | 11679046 ns 
 scalar  | double | copal::Stdlib     | sin_taylor | 11691943 ns 
 scalar  | double | copal::Stdlib     | sin_stdlib | 13933726 ns 
 simd    | double | copal::VectorImpl | sin_lookup | 15447660 ns 
 scalar  | float  | copal::Scalar     | sin_lookup | 19141175 ns 
 scalar  | float  | copal::Stdlib     | sin_lookup | 23479648 ns 
 simd    | double | copal::VectorStdx | sin_lookup | 25832231 ns 
 scalar  | double | copal::Scalar     | sin_lookup | 31361370 ns 
 scalar  | double | copal::Stdlib     | sin_lookup | 36052294 ns 


## [scalar, SIMD] float Benchmarks
 variant | type  | class             | method     | cpu time    
---------|-------|-------------------|------------|-------------
 simd    | float | copal::VectorStdx | sin_stdlib | 833680 ns   
 simd    | float | copal::VectorImpl | sin_taylor | 890045 ns   
 simd    | float | copal::VectorStdx | sin_taylor | 3769487 ns  
 simd    | float | copal::VectorImpl | sin_lookup | 4883027 ns  
 scalar  | float | copal::Stdlib     | sin_stdlib | 7050784 ns  
 simd    | float | copal::VectorStdx | sin_lookup | 10470986 ns 
 scalar  | float | copal::Stdlib     | sin_taylor | 11460499 ns 
 scalar  | float | copal::Scalar     | sin_taylor | 11594652 ns 
 scalar  | float | copal::Scalar     | sin_lookup | 19141175 ns 
 scalar  | float | copal::Stdlib     | sin_lookup | 23479648 ns 


## [scalar, SIMD] double Benchmarks
 variant | type   | class             | method     | cpu time    
---------|--------|-------------------|------------|-------------
 simd    | double | copal::VectorStdx | sin_stdlib | 2375847 ns  
 simd    | double | copal::VectorImpl | sin_taylor | 2442514 ns  
 simd    | double | copal::VectorStdx | sin_taylor | 5261707 ns  
 scalar  | double | copal::Scalar     | sin_taylor | 11679046 ns 
 scalar  | double | copal::Stdlib     | sin_taylor | 11691943 ns 
 scalar  | double | copal::Stdlib     | sin_stdlib | 13933726 ns 
 simd    | double | copal::VectorImpl | sin_lookup | 15447660 ns 
 simd    | double | copal::VectorStdx | sin_lookup | 25832231 ns 
 scalar  | double | copal::Scalar     | sin_lookup | 31361370 ns 
 scalar  | double | copal::Stdlib     | sin_lookup | 36052294 ns 


## scalar [float, double] Benchmarks
 variant | type   | class         | method     | cpu time    
---------|--------|---------------|------------|-------------
 scalar  | float  | copal::Stdlib | sin_stdlib | 7050784 ns  
 scalar  | float  | copal::Stdlib | sin_taylor | 11460499 ns 
 scalar  | float  | copal::Scalar | sin_taylor | 11594652 ns 
 scalar  | double | copal::Scalar | sin_taylor | 11679046 ns 
 scalar  | double | copal::Stdlib | sin_taylor | 11691943 ns 
 scalar  | double | copal::Stdlib | sin_stdlib | 13933726 ns 
 scalar  | float  | copal::Scalar | sin_lookup | 19141175 ns 
 scalar  | float  | copal::Stdlib | sin_lookup | 23479648 ns 
 scalar  | double | copal::Scalar | sin_lookup | 31361370 ns 
 scalar  | double | copal::Stdlib | sin_lookup | 36052294 ns 


## scalar float Benchmarks
 variant | type  | class         | method     | cpu time    
---------|-------|---------------|------------|-------------
 scalar  | float | copal::Stdlib | sin_stdlib | 7050784 ns  
 scalar  | float | copal::Stdlib | sin_taylor | 11460499 ns 
 scalar  | float | copal::Scalar | sin_taylor | 11594652 ns 
 scalar  | float | copal::Scalar | sin_lookup | 19141175 ns 
 scalar  | float | copal::Stdlib | sin_lookup | 23479648 ns 


## scalar double Benchmarks
 variant | type   | class         | method     | cpu time    
---------|--------|---------------|------------|-------------
 scalar  | double | copal::Scalar | sin_taylor | 11679046 ns 
 scalar  | double | copal::Stdlib | sin_taylor | 11691943 ns 
 scalar  | double | copal::Stdlib | sin_stdlib | 13933726 ns 
 scalar  | double | copal::Scalar | sin_lookup | 31361370 ns 
 scalar  | double | copal::Stdlib | sin_lookup | 36052294 ns 


## SIMD [float, double] Benchmarks
 variant | type   | class             | method     | cpu time    
---------|--------|-------------------|------------|-------------
 simd    | float  | copal::VectorStdx | sin_stdlib | 833680 ns   
 simd    | float  | copal::VectorImpl | sin_taylor | 890045 ns   
 simd    | double | copal::VectorStdx | sin_stdlib | 2375847 ns  
 simd    | double | copal::VectorImpl | sin_taylor | 2442514 ns  
 simd    | float  | copal::VectorStdx | sin_taylor | 3769487 ns  
 simd    | float  | copal::VectorImpl | sin_lookup | 4883027 ns  
 simd    | double | copal::VectorStdx | sin_taylor | 5261707 ns  
 simd    | float  | copal::VectorStdx | sin_lookup | 10470986 ns 
 simd    | double | copal::VectorImpl | sin_lookup | 15447660 ns 
 simd    | double | copal::VectorStdx | sin_lookup | 25832231 ns 


## SIMD float Benchmarks
 variant | type  | class             | method     | cpu time    
---------|-------|-------------------|------------|-------------
 simd    | float | copal::VectorStdx | sin_stdlib | 833680 ns   
 simd    | float | copal::VectorImpl | sin_taylor | 890045 ns   
 simd    | float | copal::VectorStdx | sin_taylor | 3769487 ns  
 simd    | float | copal::VectorImpl | sin_lookup | 4883027 ns  
 simd    | float | copal::VectorStdx | sin_lookup | 10470986 ns 


## SIMD double Benchmarks
 variant | type   | class             | method     | cpu time    
---------|--------|-------------------|------------|-------------
 simd    | double | copal::VectorStdx | sin_stdlib | 2375847 ns  
 simd    | double | copal::VectorImpl | sin_taylor | 2442514 ns  
 simd    | double | copal::VectorStdx | sin_taylor | 5261707 ns  
 simd    | double | copal::VectorImpl | sin_lookup | 15447660 ns 
 simd    | double | copal::VectorStdx | sin_lookup | 25832231 ns 
