echo "                                                 "
echo "------------------------------------------------"
echo " ####  #    # #   # #####    ##   #####  ####  ";
echo "#      #   #   # #  #    #  #  #    #   #      ";
echo " ####  ####     #   #    # #    #   #    ####  ";
echo "     # #  #     #   #####  ######   #        # ";
echo "#    # #   #    #   #   #  #    #   #   #    # ";
echo " ####  #    #   #   #    # #    #   #    ####  ";
echo "------------------------------------------------"

echo "Para que esse scrip funcione, é preciso executá-lo em um path que contenha uma pasta 'dataset'."
echo "Primeiro checaremos se o computador possui o imagemagick. Se não, ele será instalado."
sleep 1
echo "---------------------------------------------------------------------------------------------------------"
sudo apt install imagemagick
echo "----------------------------------------------------------------------------------------------------------"
echo "Criando pasta 'output' onde as imagens serão depositadas."
mkdir output_saturated

echo "----------------------------------------------------------------------------------------------------------"
echo "Começando o processo!"

mogrify -path output_saturated -gravity South -crop 320x320+0+0 +noise Gaussian -blur 2x5 -modulate 80,50 -format jpeg dataset/*.jpg

echo "Pronto!"
