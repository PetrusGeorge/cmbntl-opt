make

k=1

for instance in instances/*; do
	echo $instance >> ./output.txt
	
	echo "Processando $instance"
	echo "Instance $k of 68"
	
	./tsp ${instance} >> ./output.txt
	
	k=$(($k+1))
done

echo "-" >> ./output.txt
