## Particle Swarm Optimalization ## 
## creates a parameter list for stopos batch processes ##
import decimal

def double_range(min, max, step):
  while min < max:
    yield min
    min += step

if __name__ == '__main__':
	output ="paraset"
	file = open(output, 'w')

	cognition = range(0, 15)
	social = range(0, 15)
	runs = range(0,100)

	for sf in social: 
		for cf in cognition: 
			for rf in runs:
				line = "%.1f\t%2.1f\tc%02d_s%02d_r%02d\n" % (cf * 0.25 + 0.0, sf * 0.25 + 0.0, cf * 25 + 0, sf * 25 + 0, rf)
				file.write(line)
	
	file.close() 