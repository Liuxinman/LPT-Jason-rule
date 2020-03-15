STARTTIME=$(date +%s)

# print the 20 x 20 matrix (the avg of ratio with same m and n)
for ((m = 1; m <= 20; m++)) 
do
    for ((n = 10; n <= 200; n+=10)) 
    do
        TOTAL=0
        for ((t = 10; t <= 100; t+=10))
        do
            for ((k = 1; k <= 100; k++))
            do
                RATIO=`./a3v2 -i <instance${m}_${n}_1_${t}_${k}.txt`
                printf "$RATIO\n"
                printf "$RATIO" >~/Desktop/ratio/r${m}_${n}_1_${t}_${k}.txt
                TOTAL=`echo $TOTAL+$RATIO|bc`
            done
        done
        AVG=`echo "scale=6;$TOTAL/1000"|bc`
        printf "$AVG " >>~/Desktop/result/result.txt 
    done
    printf "\n" >>~/Desktop/result/result.txt 
done

# separated by a new line
printf "\n" >>~/Desktop/result/result.txt 

# print in one line (the avg of ratio with same m)
# 20 r(m,..) in total
for ((m = 1; m <= 20; m++)) 
do
    TOTAL=0
    for ((n = 10; n <= 200; n+=10)) 
    do
        for ((t = 10; t <= 100; t+=10))
        do
            for ((k = 1; k <= 100; k++))
            do
                read RATIO <~/Desktop/ratio/r${m}_${n}_1_${t}_${k}.txt
                TOTAL=`echo $TOTAL+$RATIO|bc`
            done
        done
    done
    AVG=`echo "scale=6;$TOTAL/20000"|bc`
    printf "$AVG " >>~/Desktop/result/result.txt
done

# separated by a new line
printf "\n\n" >>~/Desktop/result/result.txt 

# print in one line (the avg of ratio with same n)
# 20 r(..,n) in total
INTOTAL=0
for ((n = 10; n <= 200; n+=10)) 
do
    TOTAL=0
    for ((m = 1; m <= 20; m++)) 
    do
        for ((t = 10; t <= 100; t+=10))
        do
            for ((k = 1; k <= 100; k++))
            do
                read RATIO <~/Desktop/ratio/r${m}_${n}_1_${t}_${k}.txt
                TOTAL=`echo $TOTAL+$RATIO|bc`
            done
        done
    done
    INTOTAL=`echo $INTOTAL+$TOTAL|bc`
    AVG=`echo "scale=6;$TOTAL/20000"|bc`
    printf "$AVG " >>~/Desktop/result/result.txt 
done

# separated by a new line
printf "\n\n" >>~/Desktop/result/result.txt 

# print the avg of total 400 000 r
AVG=`echo "scale=6;$INTOTAL/400000"|bc`
printf "$AVG" >>~/Desktop/result/result.txt 

ENDTIME=$(date +%s)
printf "$(($ENDTIME - $STARTTIME)) seconds" >>~/Desktop/result/result.txt 
