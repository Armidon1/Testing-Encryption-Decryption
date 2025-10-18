# Testing-Encryption-Decryption
2° Homework of Cybersecurity in La Sapienza University of Rome 

**HOMEWORK** (Due Sunday, October 19, 2025, at 11:59):
1) Use the OpenSSL API to implement and test multiple encryption and decryption operations with a fixed 128-bit symmetric key, randomly generated at initialization, using CBC mode.
2) Focus on the following algorithms: AES, Camellia (used in China as a standard), and SM4
3) Compare **encryption** and **decryption** performance using three fixed input files: a 16-byte (the size of a block, you can notice the padding) text file, a 20KB text file, and a large (>2MB) binary file
4) Measure the execution times and present the result graphically (not tables, but drawings). Also, analyse and compare file sizes
5) include any source code in the report.
we can use whatever programming language, is suggested C/C++. 
Considering that report considers 3 files and for each file we have to do an encryption and decryption, for each 3 algorithm. 

**OUTPUT EXPECTED**:
---------------STARTING TESTING---------------
TYPE - ALGORITHM - FILE_SIZE - RESULT(ms)
Encryption - AES - 16 - 0.003497 
Decryption - AES - 16 - 0.001283 
Encryption - AES - 20480 - 0.019848 
Decryption - AES - 20480 - 0.009818 
Encryption - AES - 2097152 - 3.165075 
Decryption - AES - 2097152 - 1.319533 
Encryption - CAMELLIA - 16 - 0.015649 
Decryption - CAMELLIA - 16 - 0.001512 
Encryption - CAMELLIA - 20480 - 0.073708 
Decryption - CAMELLIA - 20480 - 0.086002 
Encryption - CAMELLIA - 2097152 - 8.718392 
Decryption - CAMELLIA - 2097152 - 8.620698 
Encryption - SM4 - 16 - 0.023354 
Decryption - SM4 - 16 - 0.002073 
Encryption - SM4 - 20480 - 0.125045 
Decryption - SM4 - 20480 - 0.124885 
Encryption - SM4 - 2097152 - 13.886084 
Decryption - SM4 - 2097152 - 19.867156 
----------------------------------------------
**NOTE**: The above output is just an example, the actual times may vary based on the system and implementation.

**REPORT**:
is inside the PDF file named "Report.pdf" in this directory.