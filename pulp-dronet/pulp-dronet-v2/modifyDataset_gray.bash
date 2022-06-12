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
sleep 2
echo "----------------------------------------------------------------------------------------------------------"


sudo apt install imagemagick
echo "----------------------------------------------------------------------------------------------------------"
echo "Criando pasta 'output' onde as imagens serão depositadas."
mkdir dataset/training/output_gray
echo "----------------------------------------------------------------------------------------------------------"
echo "Começando o processo!"

for picture in dataset/fine_tuning/HMB_1/images/*.png; do
    convert $picture -gravity South -crop 320x320+0+0 -grayscale Rec709Luminance -format jpeg $picture
done

echo "Pronto!"
