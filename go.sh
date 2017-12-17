make

echo "200 rounds";

for (( i = 0; i < 100; i++ )); do
    echo "$i " | tr -d '\n' && ./Game BackInBlack_ mcasellas_v5 mcasellas_v5 mcasellas_v5 --seed $i < default.cnf > default.out 2> cerr.out && grep "got top score" cerr.out;
done

echo "400 rounds";

for (( i = 0; i < 100; i++ )); do
    echo "$i " | tr -d '\n' && ./Game mcasellas_v4 mcasellas_v3 mcasellas_v3 mcasellas_v3 --seed $i < default400.cnf > default.out 2> cerr.out && grep "got top score" cerr.out;
done

echo "1000 rounds";

for (( i = 0; i < 100; i++ )); do
    echo "$i " | tr -d '\n' && ./Game mcasellas_v4 mcasellas_v3 mcasellas_v3 mcasellas_v3 --seed $i < default1000.cnf > default.out 2> cerr.out && grep "got top score" cerr.out;
done

rm -f cerr.out
