#!/bin/bash

rm BMB.txt

echo -e "\nGKD-b_1_n25_m2.txt" >> BMB.txt
bin/BMB < datos_MDD/GKD-b_1_n25_m2.txt >> BMB.txt

echo -e "\nGKD-b_2_n25_m2.txt" >> BMB.txt
bin/BMB < datos_MDD/GKD-b_2_n25_m2.txt >> BMB.txt

echo -e "\nGKD-b_3_n25_m2.txt" >> BMB.txt
bin/BMB < datos_MDD/GKD-b_3_n25_m2.txt >> BMB.txt

echo -e "\nGKD-b_4_n25_m2.txt" >> BMB.txt
bin/BMB < datos_MDD/GKD-b_4_n25_m2.txt >> BMB.txt

echo -e "\nGKD-b_5_n25_m2.txt" >> BMB.txt
bin/BMB < datos_MDD/GKD-b_5_n25_m2.txt >> BMB.txt


for i in {6..50}
do
    #echo "Ejecutando main con $i"
    echo -e "\n"GKD-b$i.txt >> BMB.txt
    bin/BMB < datos_MDD/GKD-b_$i*.txt >> BMB.txt

done