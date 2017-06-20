#!/bin/sh
die(){
    echo >&2 "$@"
    exit 1
}

#check arguments here
#usage example:  ./release.sh linux_flash_tool_v1.0
[ "$#" -gt 1 ] || die "more argument required, $# provided. Usage: $0 <release_name> <does_release(yes)>"

#release to Desktop by default.
release_dir=/release/public1/YuSu_BJ_Share/mtk81019_valid_before_20111231
qt_lib_dir=~/QtSDK/Desktop/Qt/473/gcc/lib



#check directory existence
echo "Qt library's location:"$qt_lib_dir
echo "tool release location:"$release_dir

if [ ! -d "$release_dir" ]; then
    die "release location doesn't exit: "$release_dir
fi

if [ ! -d "$qt_lib_dir" ]; then
    die "Qt library location doesn't exit: "$qt_lib_dir
fi 


base=~/Desktop
dir=$base/$1

echo "releasing to "$dir
mkdir -p $dir
cp flash_tool $dir
cp *.bin $dir
#cp launch.sh $dir
cp image_map.xml $dir
cp usb_setting.xml $dir
cp console_mode.xsd $dir

cp -dupv libeboot.* $dir
cp -dupv libflashtool.* $dir
cp -dupv libusbfinder.* $dir

#qt dir includes libQtCore, libQtGui, libXmlPatterns
cp -dupv qt $dir -r
mkdir $dir/qt -p
cp -dupv $qt_lib_dir/libQtCore.so*        $dir/qt
cp -dupv $qt_lib_dir/libQtGui.so*         $dir/qt
cp -dupv $qt_lib_dir/libQtXmlPatterns.so* $dir/qt
cp -dupv $qt_lib_dir/libQtNetwork.so*     $dir/qt
cp -dupv $qt_lib_dir/libQtXml.so*         $dir/qt

echo "Notes:" > $dir/README.txt
echo "Remember to chmod a+x for all binaries." >>$dir/README.txt
#echo "2. execute launch.sh to start flash_tool."  >>$dir/README.txt

pushd .
cd $base
tar -zcvf $base/$1.tar.gz $1

if [ "$2" == "yes" ]
then
    echo "releasing to "$release_dir
    cp $1.tar.gz $release_dir
fi

popd
#Done
echo "releasing done."
