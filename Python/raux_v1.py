# -*- coding: utf-8 -*-
"""
Created on Sat Nov 13 11:05:54 2021

@author: Nitin.N.Singh
"""

'''
the goals of making this file are:
    1. make the code modular
    2. make the code objected oriented
    3. clean up the code a bit
    4. make it reusable and extensible for additional functinalities:
        4.1 
        4.2 Advanced Functionalities (custom desired functionalities)
        
    5. Object Spring Tile System    
'''    

import numpy as np
# import gradCalculator
import logging


class SpringTileSystem(object):
    
    
    def __init__(self, n, iter_):
        
        self.n     = n
        self.iter_ = iter_
        print('Spring coupled tile system created with {0} tiles'.format(self.n))
        
      
obj = SpringTileSystem(3,4)        