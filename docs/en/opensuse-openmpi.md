# Installing OpenArray on openSUSE with OPEN MPI
1. openSUSE Leap 15.1 x86_64
2. openmpi3

## Note

1. The default version is 'openSUSE Leap 15.1 x86_64`.
2. The default installation directory is `${HOME}/install`, users can specify the path.

### Step1: Install the basic packages

The compilation relies on some basic packages. If these packages already exists on your system, please skip this step.

```shell
zypper update
zypper in -y tar gzip bzip2 wget vim make gcc gcc-c++ gcc-fortran m4 openmpi3 openmpi3-devel automake
```

Set the enviroment：

```shell
export PATH=/usr/lib64/mpi/gcc/openmpi3/bin:$PATH
export LD_LIBRARY_PATH=/usr/lib64/mpi/gcc/openmpi3/lib64/:$LD_LIBRARY_PATH
```

### Step2: Install PnetCDF

**Note** The installation directory of PnetCDF is `${HOME}/install`.

```shell
cd
wget http://cucis.ece.northwestern.edu/projects/PnetCDF/Release/pnetcdf-1.11.2.tar.gz
tar xf pnetcdf-1.11.2.tar.gz
cd pnetcdf-1.11.2
./configure --prefix=${HOME}/install --with-mpi=/usr/lib64/mpi/gcc/openmpi3
make
make install
```

## Step3: Install OpenArray

Download, compile and install：

```shell
cd
wget https://github.com/hxmhuang/OpenArray/archive/v1.0.0-beta.1.tar.gz -O OpenArray-v1.0.0-beta.1.tar.gz
tar xf OpenArray-v1.0.0-beta.1.tar.gz
cd OpenArray-1.0.0-beta.1/
PNETCDF_DIR=${HOME}/install ./configure --prefix=${HOME}/install --with-mpi=/usr/lib64/mpi/gcc/openmpi3
make
make install
```

Test：

```shell
./manual_main
```