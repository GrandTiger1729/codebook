# ./gen SEED prints a random input; WA leaves in/out/ans
for ((i = 1; ; i++)); do
  ./gen $i > in
  ./sol < in > out
  ./brute < in > ans
  cmp -s out ans || break
  echo "OK $i"
done
echo "WA on seed $i"
