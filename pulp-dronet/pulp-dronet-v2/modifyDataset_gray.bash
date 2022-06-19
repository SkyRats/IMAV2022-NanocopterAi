echo "                                                 "
echo "------------------------------------------------"
echo " ####  #    # #   # #####    ##   #####  ####  ";
echo "#      #   #   # #  #    #  #  #    #   #      ";
echo " ####  ####     #   #    # #    #   #    ####  ";
echo "     # #  #     #   #####  ######   #        # ";
echo "#    # #   #    #   #   #  #    #   #   #    # ";
echo " ####  #    #   #   #    # #    #   #    ####  ";
echo "------------------------------------------------"

echo "Primeiro checaremos se o computador possui o imagemagick. Se não, ele será instalado."
sleep 2
echo "----------------------------------------------------------------------------------------------------------"

if [ "$(which convert)" == "" ]; then
    echo "Você ainda não possui o pacote imagemagick. Instale-o a seguir:"
    sudo apt install imagemagick
else
    echo "Você já possui o pacote imagemagick"
fi

echo "----------------------------------------------------------------------------------------------------------"
echo "Começando o processo!"

for picture in `find dataset/ -regex ".*\.\(png\)"`; do
    convert $picture -gravity South -crop 200x200+0+0 -grayscale Rec709Luminance -blur 5x1 -format jpeg "${picture/.png/.jpeg}"
    rm $picture
done

for picture in `find dataset/ -regex ".*\.\(jpg\)"`; do
    convert $picture -gravity South -crop 200x200+0+0 -grayscale Rec709Luminance -blur 5x1 -format jpeg "${picture/.jpg/.jpeg}"
    rm $picture
done

for picture in `find dataset/ -regex ".*\.\(pgm\)"`; do
    convert $picture -gravity South -crop 200x200+0+0 -format jpeg "${picture/.pgm/.jpeg}"
    rm $picture
done

echo "Pronto!"
