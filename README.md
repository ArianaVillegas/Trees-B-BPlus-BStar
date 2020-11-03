# Trees-B-BPlus-BStar

Requirements
-------------
The basic requirements for this example is a anaconda enviroment:


## Installation on LINUX/UNIX Systems

Download miniconda from

https://docs.conda.io/en/latest/miniconda.html

```
chmod +x Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
conda activate
```

## Installation the following packages

```
conda install -c anaconda cmake
conda install -c conda-forge gtest
conda install -c conda-forge gmock
```

Build process
-------------

### Command-line
```
./conda/recipes/utecdf/build.sh
```

run gtest:
```
./build/bstar_tree-gtest
```

or 

```
cd /my_project_path/
mkdir build
cd build
cmake ..
make all
```

### Clion

Open cmake settings and set ENVIRONMENT to `CONDA_PREFIX=$HOME/miniconda3`. 

Finally reload de CMake Project.