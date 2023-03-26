gamma=10
final_gamma=1001
for((;gamma<final_gamma;gamma=gamma+10))
do
python poly_gen.py $gamma
./out
done
