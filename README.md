# Cosmological Calculator
## Description
A C crogram that takes in a set of RA (right ascension), Dec (declination) ans z (redshift) and returns the Comoving distance and the X, Y, Z coordinates in 3d space.

![RA & Dec on the celestial sphere](https://upload.wikimedia.org/wikipedia/commons/9/98/Ra_and_dec_on_celestial_sphere.png)
[Source:](https://commons.wikimedia.org/wiki/File:Ra_and_dec_on_celestial_sphere.png)
## Input example
```
RA,DEC,Redshift
156.37622,14.035377,0.100427
240.22965,34.764415,0.101394
229.10482,58.40596,0.10277
144.73288,47.207994,0.10352
128.30491,54.372946,0.103973
118.12623,30.268796,0.104871
184.87738,13.920614,0.10489

```

## Compilation command and example usage
gcc -o cosmo_calc.o cosmo_calc.c -lm -O3
./cosmo_calc.o ra_dec.csv ra_dec_out.csv

## Output
```
RA,DEC,Redshift,Comoving_dist,X,Y,Z
156.376220,14.035377,0.100427,420.211734,-373.502894,163.364043,101.910150
240.229650,34.764415,0.101394,424.172146,-173.018501,-302.470176,241.864426
229.104820,58.405960,0.102770,429.803266,-147.415486,-170.210023,366.098443
144.732880,47.207994,0.103520,432.862039,-240.090921,169.787081,317.644836
128.304910,54.372946,0.103973,434.696757,-156.953652,198.702783,353.332740
118.126230,30.268796,0.104871,438.364960,-178.479269,333.893985,220.961073
184.877380,13.920614,0.104890,438.446457,-424.028268,-36.183418,105.480253
199.186550,10.959153,0.105335,440.239186,-408.202766,-142.043807,83.693489
174.265830,0.471462,0.105818,442.194443,-439.966878,44.179549,3.638584
212.395590,6.173245,0.106182,443.701301,-372.475223,-236.339751,47.713469
```

## Acknowledgement
Based on the work by Professor  Edward L. Wright  - Wright (2006, PASP, 118, 1711)
https://astro.ucla.edu/~wright/CosmoCalc.html
