# -*- coding: utf-8 -*-
"""
Created on Wed Jun  3 19:04:43 2020

@author: Nitin.N.Singh
"""

# -*- coding: utf-8 -*-
"""
Created on Thu Dec  3 11:47:24 2015

@author: singh
"""

# ------------------------------------------------IMPORT ALL THE PACKAGES----------------------------------------------------------------------------
import numpy as np ;    import matplotlib ;    from matplotlib import pyplot as plt ;   from matplotlib.widgets import Slider,  Button,  RadioButtons 
import pylab ;     import matplotlib.patches as patches;     import random; import scipy ;   import math ;   import sympy as sp ; from sympy import * 
from scipy.spatial import distance ; import time; start_time = time.time()
# ---------------------------------------------------------------------------------------------------------------------------------------------------

#----------------------------------------THESE NOTATIONS MAKE THE WRITIGN EASIER---------------------------------------------------------------------
pi = np.pi ;dot = np.dot ; sin = np.sin ; cos = np.cos ; ar = np.array; sqrt = np.sqrt; rand = scipy.rand ; arange = scipy.arange ; show = pylab.show
plot = pylab.plot ;  axis = pylab.axis ; grid = pylab.grid ;  title  = pylab.title ; atan = np.arctan; transpose = np.transpose ; dotProduct = np.dot
# ---------------------------------------------------------------------------------------------------------------------------------------------------


#---------------------------------------------MAIN VISUALIZATION FUNCTION----------------------------------------------------------------------------  
def visualize(position_vector, n, shape_info_bodies):
    
    for i in range(0, n**2-1):
               
        clr_list = ['#ff0000', '#0000ff', '#f9da13', '#008000']
        clr      = clr_list[(i%4)]
        corner_body = np.array([[l*np.cos(theta+position_vector[2+3*i,0])+position_vector[0+3*i,0], l*np.sin(theta+position_vector[2+3*i,0])+\
                                                                    position_vector[1+3*i,0]] for l,theta in zip(shape_info_bodies[i, :, 0],shape_info_bodies[i, :, 1])]) 
        #pylab.gca().add_patch(patches.Polygon(corner_body,closed=True,fill=True, color = '#36648B', alpha = transparency, lw=1))
        pylab.gca().add_patch(patches.Polygon(corner_body,closed=True,fill=True, color = clr, alpha = 1,lw=1)) 
        #pylab.gca().add_patch(patches.Polygon(corner_body,closed=True,fill=True, color = 'blue', alpha = transparency, lw=1))                                                            
               
        #print('line check line check')
                
         
        
        #if(i==5 or i==7):
        plt.plot(corner_body[:,0], corner_body[:,1], 'o', color = 'white', markeredgecolor='black', markersize = 5) 
        #temp_string = "%.02e"%(testMat1_[count_])
        #plt.plot(corner_body[:,0], corner_body[:,1], 'o', color = 'black', markersize = 3, label = r'$f$' + ' = ' + temp_string if i ==0 else '')        
                
        #pylab.gca().add_patch(patches.Polygon(corner_body,closed=True,fill=True, color = 'yellow', alpha = 0.80))                                                            
        
        
        #plt.text(position_vector[0+3*i,0], position_vector[1+3*i,0], i, size=7, horizontalalignment='center')                             # BODY NAME
        
        '''
        #------------------------------------ANGLE COLOR AND NAME--------------------------------------------------
        for j in range(4):
            l1xy = np.array([[corner_body[j,0], corner_body[j,1]],[corner_body[j-1,0], corner_body[j-1,1]]])
            l2xy = np.array([[corner_body[j,0], corner_body[j,1]],[corner_body[j-3,0], corner_body[j-3,1]]])
            angle_plot = get_angle_plot(l1xy, l2xy, j, 10, 0.50, origin = [corner_body[j,0], corner_body[j,1]]) 
            pylab.gca().add_patch(angle_plot) 
            plt.text((position_vector[0+3*i,0]+corner_body[j,0])/2, (position_vector[1+3*i,0]+corner_body[j,1])/2,\
                                            j, size=7, horizontalalignment='center',color = 'blue')
        #----------------------------------------------------------------------------------------------------------   
        '''  

        x7 = shape_info_bodies[7, 3, 0]*np.cos(shape_info_bodies[7, 3, 1]+position_vector[2+3*7,0])+position_vector[0+3*7,0] 
        y7 = shape_info_bodies[7, 3, 0]*np.sin(shape_info_bodies[7, 3, 1]+position_vector[2+3*7,0])+position_vector[1+3*7,0]
        x5 = shape_info_bodies[5, 2, 0]*np.cos(shape_info_bodies[5, 2, 1]+position_vector[2+3*5,0])+position_vector[0+3*5,0]
        y5 = shape_info_bodies[5, 2, 0]*np.sin(shape_info_bodies[5, 2, 1]+position_vector[2+3*5,0])+position_vector[1+3*5,0]
        #---------------------------------------------------------------------------------------    
        
        '''
        #--------THETA LINE IN THE MECHANISM------------------------------------------------
        x = position_vector[3,0]; y = position_vector[4,0] 
        angle = position_vector[5,0] #if position_vector[5,0] >= 0 else 2*pi + position_vector[5,0] 
        x1 = x + 0.6*np.cos(angle); y1 = y + 0.6*np.sin(angle)
        x2 = x - 0.6*np.cos(angle); y2 = y - 0.6*np.sin(angle)
        plot([x, x1], [y, y1], color='k', linestyle='-', linewidth=2)
        
        plot([x, x+0.6], [y, y], color='k', linestyle='--', linewidth=2)
        
    
        #-----------------------------------------------------------------------------------
        
        l1xy = np.array([[x+1, y], [x,y]])
        l2xy = np.array([[x1,y1], [x,y]])          
        
        #get_angle_plot(l1xy, l2xy, 4, 2, 1.5, origin = [x,y])
        plt.annotate(s='', xy=(x,y), xytext=(x1,y1), arrowprops=dict(arrowstyle='<-', linestyle='solid'))
        '''
        
        
        #plt.xticks([-10, -5, 0, 5, 10], fontsize = 10); plt.yticks([-5,  0, 5, 10], fontsize = 10) 
                                 
        
        plt.xlim((-3.5,3.5))
        plt.ylim((-3.5,3.5))
        
        #plt.ylim([-2.25, 2.25])        
        #plt.gca().set_aspect('equal')
        ax = plt.gca()
        #ax.yaxis.set_ticks([-2, -1, 0, 1, 2]) 
        #ax.xaxis.set_ticks([-2, -1, 0, 1, 2]);        
        #plt.ylim([-2.5, 2.5]); plt.xlim([-2.5, 2.5])
#----------------------------------------------------------------------------------------------------------------------------------------------------
 

                        
                        
                        
        