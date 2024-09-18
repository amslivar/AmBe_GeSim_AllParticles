from math import sqrt

def calculate_rate_4438(nevts, eff, time, error_nevts, error_eff):
    rate_4438 = nevts / (eff * time)
    error_rate_4438 = rate_4438 * sqrt((error_nevts / nevts)**2 + (error_eff / eff)**2 )
    return rate_4438, error_rate_4438

nevts = 2391. # number of events in the 4.4 MeV peak from GeIII DATA
error_nevts = 59.9
eff =  0.008962 # fraction how many gammas are detected as 4.4 MeV from SIM 
error_eff = 9.46678e-05
time = 25550.1 # time from GeIII data taking in seconds, ~6h 


rate_4438, error_rate_4438 = calculate_rate_4438(nevts, eff, time, error_nevts, error_eff)
print("Rate of 4.4 MeV gammas: %s +- %s Hz" % (rate_4438,error_rate_4438))
#Rate of 4.4 MeV gammas: 10.3065771647 +- 0.826487549577 s 

#the neutron rate for AmBe-3 is 18.9 +-0.8 s^-1, from table 2 in MF report 
gamma_to_neutron = rate_4438 / 18.9  
#print("gamma to neutron rate: %s" % gamma_to_neutron)


# portion of gamma radiation that can penetrate all three encapsulation layers EMERGENT/PRIMARY 
# from /home/amslivar/work/AmBe_v2/AmBe_characterization_analysis/AmBe_emergent_gammas_exact_geometry_with_outer_W_cap
# and /home/amslivar/work/AmBe_v2/AmBe_characterization_analysis/AmBe_emergent_gammas_exact_geometry_with_outer_W_cap/Analysis_emergent_gammas_wo_4438keV_1e7.ipynb
em_gamma_prop_am = 0.0051248
#em_gamma_prop_ambe = 0.0286201
##em_gamma_prop_4438 = 0.68983


#activity of AmBe 3, according to table 3, pg 18 from report 
activity = 35.49 * 3.7e4 # in seconds 
error_activity = 0.45 * 3.7e4

# only primary gammas with > 59 keV energy were simulated (0.075% of total 241Am gamma emission or 0.076% of total 241AmBe gamma emission)

em_gamma_rate_am = em_gamma_prop_am * 0.075/100 * activity
#em_gamma_rate_ambe = em_gamma_prop_ambe * 0.076/100 * activity
#em_gamma_rate_4438 = em_gamma_prop_4438 * 0.076/100 * activity

em_gamma_rate_am_error =em_gamma_prop_am * 0.075/100 * error_activity
#em_gamma_rate_ambe_error = em_gamma_prop_ambe * 0.076/100 * error_activity  
#em_gamma_rate_4438_error = em_gamma_prop_4438 * 0.076/100 * error_activity

print("Emergent gamma rate, 241Am only: %s +- %s Hz" % (em_gamma_rate_am, em_gamma_rate_am_error))
#print("Emergent gamma rate, AmBe: %s +- %s Hz" % (em_gamma_rate_ambe, em_gamma_rate_ambe_error))
#print("Emergent gamma rate, 4438 keV only: %s +- %s Hz" % (em_gamma_rate_4438, em_gamma_rate_4438_error))