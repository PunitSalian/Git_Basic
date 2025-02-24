# My_Git

This repository aims to build and replicate Git functionality using **C++** and the **Bazel** build system.

## Design Overview
For a detailed explanation of the design and implementation, refer to the blog post:  
[Building Git from Scratch](https://psalian268274853.wordpress.com/2025/02/22/hmm-let-me-build-git/)

## Features Implemented
This project attempts to implement core Git functionalities, including:
- `init` – Initialize a new repository  
- `commit` – Record changes to the repository  
- `add` – Add files to the staging area  
- `clone` – Clone an existing repository  
- `branch` – Manage branches in the repository  

## Running Tests
To execute the test suite, run the following command:  
```sh
bazel test //...