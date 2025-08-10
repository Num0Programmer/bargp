if [ -z $1 ]; then
    sudo make install
else
    make -e INSTALL_PREFIX=$1 install 
fi
