from math import sqrt

def calculate_result(nevts, eff, time, error_nevts, error_eff):
    result = nevts / (eff * time)
    error_result = result * sqrt((error_nevts / nevts)**2 + (error_eff / eff)**2 )
    return result, error_result

# Example usage
nevts = 2132.
error_nevts = 83.9
eff =  0.008962 
error_eff = 9.46678e-05
time = 25550.


result, error_result = calculate_result(nevts, eff, time, error_nevts, error_eff)
print("Rate: %s s" % result)
print("Error: %s" % error_result)

gamma_to_neutron = result / 18.9  

print("gamma to neutron rate: %s" % gamma_to_neutron)

# 0.577 0.029 gamma to neutron rate
#gamma to neutron rate: 0.49263986453 calculated here with new gamma rate, old neutron 

#18.9 0.8 neutron rate form He3

# Rate: 9.31 +- 0.38 s 

#Rate: 9.31089343962 s
#Error: 0.379379633972