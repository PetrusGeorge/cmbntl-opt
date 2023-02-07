make

k=1

for instance in instances/*; do
	echo $instance >> ./output.txt
	
	echo "Processando $instance"
	echo "Instance $k of 22"
	
	./mlp ${instance} >> ./output.txt
	
	k=$(($k+1))
done

echo "-" >> ./output.txt
