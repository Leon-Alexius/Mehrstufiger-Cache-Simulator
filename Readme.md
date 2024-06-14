# Cache Simulation Project

## Overview
This project aims to investigate the impact of multi-level caches on runtime and latency. It involves researching cache sizes, architectures, and latencies in modern processors, and implementing a cache simulation in SystemC with C++ and a framework program in C.

## Requirements

| Language | Version |
|----------|---------|
| C        | C17     |
| C++      | C++14   |
| SystemC  | 2.3.3   |

## Installation
Provide instructions on how to install and set up your project here.

## Usage
Provide instructions on how to use your project here.

## Theoretical Results
Document the results of the theoretical part of your project here. This could include findings from your research on cache sizes, architectures, and latencies in modern processors.

## Personal Contributions
### Member 1
Briefly describe the contributions of the first group member here.

### Member 2
Briefly describe the contributions of the second group member here.

### Member 3
Briefly describe the contributions of the second group member here.

## Compilation
All code here needs to be able to be compiled in the remote ssh machine. To avoid any problems, editing, testing, and running the code should be done on the ssh machine.

## Unit Testing
Running tests everytime you want to push a feature is recommended. For that an option would be to use [doctest](https://github.com/doctest/doctest). However, I don't really know how to do that yet :'

## IDE/Code Editor
A good code editor for this project is VSCode as it is lightweight and can be easily used in the remote ssh machine.

## Contributing
To achieve a seamless and easy communication and cooperation between us, some convention/workflow needs to be followed, which can be seen here: https://medium.com/anne-kerrs-blog/using-git-and-github-for-team-collaboration-e761e7c00281.

#### Branches
It is generally good practice to push a new feature to a new _branch_ in git. By doing so, each branch will need to be reviewed before merging it into `main`. The reason for this is to generally always have a working `main` branch. I suggest that every time a branch is pushed by someone, 2 other people would review and approve it.

It is good to follow some sort of naming convention for the branches. A new branch should have the name: `your_name`/`feature`

Pushing a new branch through the terminal (let's say pushing a new module):

1. Create a new branch
    ```
    $ git checkout -b anthony/moduleA
    ```
2. Create the feature (in `.hpp`):
    ```C++
    SC_MODULE(MODULE_A) {
      ...
    }
    ```
3. Make sure that the feature could be compiled and runs properly
    ```
    $ make
    ```
4. Commit and push
    ```
    $ git add file_name (do not add more things which are not important to the feature!)
    $ git commit -m "added ModuleA"
    $ git push (if you are pushing for the first time do $ git push --set-upstream origin anthony/moduleA)
    ```
5. Create a pull request
6. Resolve merge conflicts which may arise
7. Wait for the pull request to be approved by another team member
8. After the review the pull request will be merged into the main branch and the feature branch will be deleted
9. Optional: delete the branch locally
    ```
    $ git branch -d anthony/moduleA
    ```
10. Pull latest changes of the main branch locally (including your recently merged feature branch)
    ```
    $ git checkout main
    $ git pull origin main
    ```

#### GitHub Actions
With GitHub Actions, pushing a new feature would set off a chain of actions which sets up a linux machine, compiles the code, and runs our written tests. To be set up.

#### Code Style
It is recommended to indent a code line with two spaces. We do this because a tab `\t` does not always have the same length from machine to machine. You can set this up by pressing `F1` and search for `Indent Using Spaces`. Then choose `2` as the indent size.

We should also comment code which we have written. A good convention would be to first of all write your name, and then describe what it does. Better yet, you could also write for each line what it does. Example:
```C++
/*anthony
(code description)
*/
```

## License
All Rights Reserved © 2024 Team-150.

This project and its contents are the property of Team-150. No reproduction, distribution, or transmission of the copyrighted materials in this project is permitted without the written permission of Team-150.

## Contact
Include your contact information here.

## Acknowledgements
Include any acknowledgements here.
