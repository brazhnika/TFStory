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

rm -r ./assets
rm -r ./cfg

cp -r ./../assets ./assets
cp -r ./../cfg ./cfg/
rm ./cfg/*.cpp
rm ./cfg/*.h

