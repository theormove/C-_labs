from pyfaidx import Fasta



def hamming_distance(string1, string2):  # returns hamming distance for smaller string and first part of the second one and assuming that insertion, deletion and substitution have same value adding difference in length to the distance
    distance = 0
    L = min(len(string1), len(string2))
    for i in range(L):
        if str(string1[i]) != str(string2[i]):
            distance += 1
    return distance + abs(len(string1) - len(string2))

def q_matr_calc(matr): # calculates Q matrix for neghbor joining clustering method 
	q_matr = []
	q_matr.append(matr[0])
	for i in range(len(matr)-1):
		row = []
		for j in range(len(matr[0])):
				row.append((len(matr[0]) - 2)*matr[i+1][j] - sum_line(matr, i+1) - sum_line(matr, j+1))
		q_matr.append(row)		
	return q_matr			 

def sum_line(matr, line): #sums line in matrix 
	sum_line = 0
	for i in range(len(matr[0])):
		sum_line += matr[line][i]
	return sum_line	

def print_array(ar): # prints array (just for testing)
	for i in range(len(ar)):
		for j in range(len(ar[i])):
			print(ar[i][j], end=' ')
		print() 		

def find_min(matr): #finds minimum value in symmetric matrix (diagonal elements not included) 
	ind = []
	l = len(matr[0])
	for i in range(l-1):
		ind.append(min(matr[i+1][i+1:]))		
	min0 = min(ind)	
	for i in range(l):
		try:
			j = matr[i+1][i+1:].index(min0)
			return [i+1,i+1+j]
		except:
			pass
	
def neighbor_joining(matr): #Neighbor joining clustering function
	q_matr = q_matr_calc(matr)
	ind = find_min(q_matr)
	new_row = []
	l = len(matr[0])
	if l <= 3:
		return "Process completed"	
	for i in range(1,l+1):  #evaluating cluster distances 
		if i != ind[0] and i != ind[1]+1:
			new_row.append((matr[i][ind[0]-1] + matr[i][ind[1]] - matr[ind[0]][ind[1]])/2)
	new_name = matr[0][ind[0]-1] + ' , ' + matr[0][ind[1]]
	new_row.append(0)
	for i in range(l+1): #deleting 2 taxons that were clustered 
		del matr[i][max(ind[0]-1, ind[1])]
		del matr[i][min(ind[1],ind[0]-1)]
	del matr[max(ind[0], ind[1]+1)]
	del matr[min(ind[1]+1,ind[0])]	
	matr[0].append(new_name) # adding cluster to distance matrix
	for i in range(1,l-1):
		matr[i].append(new_row[i-1])
	matr.append(new_row)
	print()	
	print(new_name)
	return neighbor_joining(matr)				

def UPGMA(matr): #UPGMA clustering function
	ind = find_min(matr)
	new_row = []	
	l = len(matr[0])
	if l <= 2:
		return "Process completed"		
	for i in range(1,l+1):
		if i != ind[0] and i != ind[1]+1: #evaluating cluster distances 
			new_row.append((matr[i][ind[0]-1]*(matr[0][i-1].count(',') + matr[0][ind[0]-1].count(',') + 1) + matr[i][ind[1]]*(matr[0][i-1].count(',') + matr[0][ind[1]].count(',') + 1))/(matr[0][i-1].count(',')*2+matr[0][ind[1]].count(',')+ 2 +matr[0][ind[0]-1].count(',')))
	new_name = matr[0][ind[0]-1] + ' , ' + matr[0][ind[1]]
	new_row.append(0)
	for i in range(l+1):  #deleting 2 taxons that were clustered 
		del matr[i][max(ind[0]-1, ind[1])]
		del matr[i][min(ind[1],ind[0]-1)]	
	del matr[max(ind[0], ind[1]+1)]
	del matr[min(ind[1]+1,ind[0])]	
	matr[0].append(new_name) # adding cluster to distance matrix
	for i in range(1,l-1):
		matr[i].append(new_row[i-1])
	matr.append(new_row)	
	print(new_name)
	print()
	return UPGMA(matr)
if __name__ == "__main__":
	fasta = Fasta("aquifex-tRNA.fasta")	#reading from file using pyfaidx library
	header3 =fasta.keys()
	headers = []
	for header in header3:
		headers.append(str(header))
	dist_matrix = [] 
	dist_matrix.append(headers)
	for header1 in headers:
		row = []
		for header2 in headers:
			row.append(hamming_distance(fasta[header1], fasta[header2]))
		dist_matrix.append(row)			
	UPGMA(dist_matrix)
	#neighbor_joining(dist_matrix)				
	#UPGMA([['a','b','c','d','e'],[0,17,21,31,23],[17,0,30,34,21],[21,30,0,28,39],[31,34,28,0,43],[23,21,39,43,0]])
	#neighbor_joining([['a','b','c','d','e'],[0,5,9,9,8],[5,0,10,10,9],[9,10,0,8,7],[9,10,8,0,3],[8,9,7,3,0]])