import numpy as np

def convert_file(data, f):
	for arr in data: f.write("\n{}\t\t{}".format(arr[0], arr[1]))


if __name__ == '__main__':
	output ="input.dat"
	n_files = 50
	n_vertices = 12
	
	file = open(output, 'w')
	file.write("connected_vertices=8\n0\t1\n1\t2\t9\n2\t3\n")
	file.write("0\t7\t8\n8\t9\t10\t11\n3\t4\t10\n6\t7\n5\t6\t11\n")

	file.write("\nparticle_count={}\n".format(n_files))
	file.write("vertices_count={}".format(n_vertices))
	for index in range(0, n_files):
		file.write("\n\nparticle={}".format(index))
		np_file = "particle{}.npy".format(index)
		data = np.load(np_file)
		convert_file(data, file)

	file.close()