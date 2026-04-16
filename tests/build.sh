if [ -d "./build" ]; then
   echo "Directory ./build exists."
else
   echo "Creating directory ./build"
   mkdir build
fi

cd build
cmake ..
#cmake .
cmake --build .

