# -*- coding: utf-8 -*-



# ----------------------------------------------------USED PACKAGES-----------------------------------------------------
import numpy as np;import matplotlib;from matplotlib import pyplot as plt;import pylab;from scipy.spatial import distance       
import matplotlib.patches as patches;import random; import scipy; import math; import sympy as sp#; from sympy import  * 
from scipy.spatial  import distance ; import time; start_time = time.time(); import os; import profile
from math import atan2
from visualize import visualize
from   numba  import njit  
# ----------------------------------------------------------------------------------------------------------------------


#----------------------------------------------THESE NOTATIONS MAKE THE WRITIGN EASIER----------------------------------
pi = np.pi ;   dot = np.dot ;    sin = np.sin ;    cos = np.cos ;    ar = np.array ;  sqrt = np.sqrt; rand = scipy.rand  
arange = scipy.arange; show = pylab.show; plot = pylab.plot; axis = pylab.axis; grid = pylab.grid; title  = pylab.title 
atan = np.arctan;transpose = np.transpose;dotProduct = np.dot;conc = np.concatenate;lnsp = np.linspace; log10 = np.log10
dist = np.linalg.norm
#-----------------------------------------------------------------------------------------------------------------------

#plt.close('all')
t0 = time.time()
#--------------------------------------------------------------OOP PART-------------------------------------------------

class UpdateNetwork(object):
    
    
    def __init__(self, n_tiles, X1, Y1, Theta1, LenVec1, Phi1, X2, Y2, Theta2, LenVec2, Phi2, BCPM2D, BCPM3D): 
        """Initialize the network by defining the current position vector, the connectivity among the members, the shape
        information of the members. Preallocate the First Derivative Vector(FDV) and the  Second  Derivative Matrix(SDM) 
        for the network."""
        
        self.n_tiles = n_tiles
        self.X1 = X1; self.Y1 = Y1; self.Theta1 = Theta1; self.LenVec1 = LenVec1; self.Phi1 = Phi1
        self.X2 = X2; self.Y2 = Y2; self.Theta2 = Theta2; self.LenVec2 = LenVec2; self.Phi2 = Phi2       
        self.BCPM2D = BCPM2D
        self.BCPM3D = BCPM3D        
        self.FDV  = np.zeros((self.X1.shape[0], 6))
        self.SDM  = np.zeros((self.X1.shape[0], 6, 6))      
        
    
    def UpdateFirstDerivativeVector(self):
        """Fill  the appropriate elements of FDV by considering every connection in the network  one by one. Derivatives 
        are calculated analytically."""
        
        X1 = self.X1; Y1 = self.Y1; Theta1 = self.Theta1; LenVec1 = self.LenVec1; Phi1 = self.Phi1  
        X2 = self.X2; Y2 = self.Y2; Theta2 = self.Theta2; LenVec2 = self.LenVec2; Phi2 = self.Phi2          
            
        self.FDV[:, 0]     =  2*X1 - 2*X2 + 2*LenVec1*cos(Theta1 + Phi1) - 2*LenVec2*cos(Theta2 + Phi2)
                                                                           
        self.FDV[:, 1]     =  2*Y1 - 2*Y2 + 2*LenVec1*sin(Theta1 + Phi1) - 2*LenVec2*sin(Theta2 + Phi2)
        
        self.FDV[:, 2]     =  2*LenVec1*(-X1 + X2 - LenVec1*cos(Theta1 + Phi1) + LenVec2*cos(Theta2 + Phi2))\
                                                *sin(Theta1 + Phi1) - 2*LenVec1*(-Y1 + Y2 - LenVec1*sin(Theta1 + Phi1) +\
                                                LenVec2*sin(Theta2 + Phi2))*cos(Theta1 + Phi1)
        
        self.FDV[:, 3]     =  -2*X1 + 2*X2 - 2*LenVec1*cos(Theta1 + Phi1) + 2*LenVec2*cos(Theta2 + Phi2)
        
        self.FDV[:, 4]     =  -2*Y1 + 2*Y2 - 2*LenVec1*sin(Theta1 + Phi1) + 2*LenVec2*sin(Theta2 + Phi2)
        
        self.FDV[:, 5]     =  -2*LenVec2*(-X1 + X2 - LenVec1*cos(Theta1 + Phi1) + LenVec2*cos(Theta2 + Phi2))\
                                                *sin(Theta2 + Phi2) + 2*LenVec2*(-Y1 + Y2 - LenVec1*sin(Theta1 + Phi1) +\
                                                LenVec2*sin(Theta2 + Phi2))*cos(Theta2 + Phi2) 
        
        self.FDV = self.FDV.flatten()[(np.cumsum(self.BCPM2D).reshape(self.BCPM2D.shape)-1)*self.BCPM2D] * self.BCPM2D
        self.FDV = np.sum(self.FDV, axis = 0)  
        self.FDV = self.FDV.reshape(self.n_tiles*3,1)
        
#        self.FDV[44,0] = 0.0
#        self.FDV[47,0] = 0.0
#        self.FDV[46,0] = 0.0; self.FDV[45,0] = 0.0 
        
        self.FDV[5,0] = 0.0
        self.FDV[14,0] = 0.0
        self.FDV[12,0] = 0.0; self.FDV[13,0] = 0.0 
        return self.FDV 
     
    
    def UpdateSecondDerivativeMatrix(self):
        """Fill the appropriate elements of SDM by considering every connection in the network one by one. Derivatives 
        are calculated analytically."""
        X1 = self.X1; Y1 = self.Y1; Theta1 = self.Theta1; LenVec1 = self.LenVec1; Phi1 = self.Phi1  
        X2 = self.X2; Y2 = self.Y2; Theta2 = self.Theta2; LenVec2 = self.LenVec2; Phi2 = self.Phi2
        
        self.SDM[:,0,0]   +=  2.0 
        self.SDM[:,1,1]   +=  2.0
        self.SDM[:,3,3]   +=  2.0
        self.SDM[:,4,4]   +=  2.0
        
        self.SDM[:,0,3]   += -2.0               
        self.SDM[:,3,0]   += -2.0
        self.SDM[:,1,4]   += -2.0 
        self.SDM[:,4,1]   += -2.0
        
        self.SDM[:,0,1] = self.SDM[:,1,0] = self.SDM[:,0,4] = self.SDM[:,4,0] = self.SDM[:,1,3] = self.SDM[:,3,1] = \
        self.SDM[:,3,4] = self.SDM[:,4,3] = 0.0
                                    
        self.SDM[:,0,2]   += -2*LenVec1*sin(Theta1 + Phi1)
        self.SDM[:,2,0]   += -2*LenVec1*sin(Theta1 + Phi1) 
        self.SDM[:,0,5]   +=  2*LenVec2*sin(Theta2 + Phi2)
        self.SDM[:,5,0]   +=  2*LenVec2*sin(Theta2 + Phi2)
        self.SDM[:,1,2]   +=  2*LenVec1*cos(Theta1 + Phi1)
        self.SDM[:,2,1]   +=  2*LenVec1*cos(Theta1 + Phi1) 
        self.SDM[:,1,5]   += -2*LenVec2*cos(Theta2 + Phi2)
        self.SDM[:,5,1]   += -2*LenVec2*cos(Theta2 + Phi2) 
        
        self.SDM[:,2,2]    +=   2*LenVec1*(LenVec1*sin(Theta1 + Phi1)**2 + LenVec1*cos(Theta1 + Phi1)**2 - (X1 - X2 + \
                                LenVec1*cos(Theta1 + Phi1)- LenVec2*cos(Theta2 + Phi2))*cos(Theta1 + Phi1) - (Y1 - Y2 +\
                                LenVec1*sin(Theta1 + Phi1) - LenVec2*sin(Theta2 + Phi2))*sin(Theta1 + Phi1))
        
        self.SDM[:,2,3]    +=  2*LenVec1*sin(Theta1 + Phi1)
        self.SDM[:,3,2]    +=  2*LenVec1*sin(Theta1 + Phi1) 
        self.SDM[:,2,4]    += -2*LenVec1*cos(Theta1 + Phi1)
        self.SDM[:,4,2]    += -2*LenVec1*cos(Theta1 + Phi1)
        
        self.SDM[:,2,5]    += -2*LenVec1*LenVec2*(sin(Theta1+Phi1)*sin(Theta2+Phi2) + cos(Theta1+Phi1)*cos(Theta2+Phi2))
        self.SDM[:,5,2]    += -2*LenVec1*LenVec2*(sin(Theta1+Phi1)*sin(Theta2+Phi2) + cos(Theta1+Phi1)*cos(Theta2+Phi2))
        
        self.SDM[:,3,5]    += -2*LenVec2*sin(Theta2 + Phi2) 
        self.SDM[:,5,3]    += -2*LenVec2*sin(Theta2 + Phi2) 
        self.SDM[:,4,5]    +=  2*LenVec2*cos(Theta2 + Phi2)
        self.SDM[:,5,4]    +=  2*LenVec2*cos(Theta2 + Phi2)
        
        self.SDM[:,5,5]    +=  2*LenVec2*(LenVec2*sin(Theta2 + Phi2)**2 + LenVec2*cos(Theta2 + Phi2)**2 + (X1 - X2 +\
                                 LenVec1*cos(Theta1 + Phi1)- LenVec2*cos(Theta2 + Phi2))*cos(Theta2 + Phi2) + (Y1 - Y2\
                                 + LenVec1*sin(Theta1 + Phi1) -LenVec2*sin(Theta2 + Phi2))*sin(Theta2 + Phi2))
        
                                  
        self.SDM = self.SDM.flatten()[(np.cumsum(self.BCPM3D).reshape(self.BCPM3D.shape)-1)*self.BCPM3D] * self.BCPM3D  
        self.SDM = np.sum(self.SDM, axis = 0)
        
        
#        self.SDM[44, :] = self.SDM[:, 44] = 0.0
#        self.SDM[47, :] = self.SDM[:, 47] = 0.0
#        self.SDM[46, :] = self.SDM[:, 46] = 0.0; self.SDM[45, :] = self.SDM[:, 45] = 0.0 
        
        self.SDM[5, :] = self.SDM[:, 5] = 0.0
        self.SDM[14, :] = self.SDM[:, 14] = 0.0
        self.SDM[12, :] = self.SDM[:, 12] = 0.0; self.SDM[13, :] = self.SDM[:, 13] = 0.0 
        return self.SDM


    def UpdateEnergyInSprings(self):
        X1 = self.X1; Y1 = self.Y1; Theta1 = self.Theta1; LenVec1 = self.LenVec1; Phi1 = self.Phi1  
        X2 = self.X2; Y2 = self.Y2; Theta2 = self.Theta2; LenVec2 = self.LenVec2; Phi2 = self.Phi2
        
        self.Energy = ((LenVec1*np.cos(Phi1+Theta1)+X1)-(LenVec2*np.cos(Phi2+Theta2)+X2))**2+((LenVec1*np.sin(Phi1+\
                               Theta1)+Y1)-(LenVec2*np.sin(Phi2+Theta2)+Y2))**2
        #print(self.Energy)
        self.Energy = np.sum(self.Energy, axis = 0)   

        #print(self.Energy )             
        return self.Energy                        
#-----------------------------------------------------------------------------------------------------------------------



# @njit 
def main_func():
    #### The code right now is not extensible to any general n, few changes needed??
    
    #pts = pts_hold + 0.1*np.random.randn(60,2)
    
    #pts = np.array(([[0,2],[-1,1],[],[]])) 
            
    n = 3 
    n_tiles = n**2
    n_conxn =  int((2*4  +  3*4*(n-2)  +  4*(n**2 - 4*(n-2) -4))/2) 
    
    n_line_tile = 4
    n_trig_tile = 4*(n-2)
    n_quad_tile = n_tiles - 4*(n - 2) - 4
    
    tile_netwrk = []
    for i in range(n):
        if i==0 or i==n-1:
            for j in range(n):
                if j==0 or j==n-1:
                    if i==0 and j==0:
                        tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'lineLT', 
                                           'tile_vertices':(0,n-1)})
                    elif i==0 and j ==n-1:
                        tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'lineRT', 
                                           'tile_vertices':(n-2, 2*n-2)})
                    elif i==n-1 and j==0:
                        tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'lineLB', 
                                           'tile_vertices':(n_conxn-n+1, n_conxn-n+1-n)})
                    else:
                        tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'lineRB', 
                                           'tile_vertices':(n_conxn-1, n_conxn-n)})
                else:
                    if i==0:
                        tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'triHU', 
                                            'tile_vertices':(j-1,j,j+n-1)})
                    else:
                        tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'triHL', 
                                            'tile_vertices':(n_conxn-n+1+j-1,n_conxn-n+1+j
                                                         ,n_conxn-n+1+j-1-n+1)})
        else:            
            for j in range(n):
                if j==0: 
                    tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'triVL', 
                                       'tile_vertices':(n*i + (n-1)*i-n, n*i + (n-1)*i,
                                                     n*i + (n-1)*i+n-1)})
                elif j==n-1:
                    tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'triVR', 
                                       'tile_vertices':(n*i + (n-1)*i + n-2 - n +1, 
                                                     n*i + (n-1)*i + n-2, 
                                                     n*i + (n-1)*i + n-2+n)})
                else:
                    tile_netwrk.append({'tile_idx':n*i+j, 'tile_type':'quad', 
                                       'tile_vertices':(n*i + (n-1)*i + j-1, 
                                                     n*i + (n-1)*i + j, 
                                                     n*i + (n-1)*i + j-1 - n+1, 
                                                     n*i + (n-1)*i + j-1+n)})
    
    
        
    #-----------------------------------INITIALIZE THE POSITION VECTOR OF THE PARTICLES-------------------------------------
    '''SPV encodes the position and orientation of the polygons. Preallocate the position vector. Fill in 
    the appropriate places of the vector and randomize things a bit.'''                                
    pos_vec = np.zeros((3*(n_tiles), 1), dtype = float)   
                                             
    pts = np.random.rand(n_conxn,2)
    
    r'''
    SPV[0],  SPV[1]  = (pts[0]+pts[1])/2;    SPV[2] = 0
    SPV[3],  SPV[4]  = (pts[1]+pts[2]+pts[9])/3;  SPV[5] = 0                            
    SPV[6],  SPV[7]  = (pts[2]+pts[3])/2;  SPV[8] = 0
    SPV[9],  SPV[10] = (pts[0]+pts[7]+pts[8])/3; SPV[11] = 0    
    SPV[12], SPV[13] = (pts[8]+pts[9]+pts[10]+pts[11])/4; SPV[14] = 0    
    SPV[15], SPV[16] = (pts[3]+pts[4]+pts[10])/3; SPV[17] = 0 
    SPV[18], SPV[19] = (pts[6]+pts[7])/2; SPV[20] = 0
    SPV[21], SPV[22] = (pts[5]+pts[6]+pts[11])/3;  SPV[23] = 0
    #-----------------------------------------------------------------------------------------------------------------------
    '''
    
    for i in range(n_tiles):
        tile = tile_netwrk[i]
        pos_vec[3*(i+1)-3]   = np.mean([pts[i,0] for i in tile['tile_vertices']])
        pos_vec[3*(i+1)-3+1] = np.mean([pts[i,1] for i in tile['tile_vertices']])
        pos_vec[3*(i+1)-3+2] = 0
    
    #%%
    r'''General Function to allocate values to position vector'''    
    # tile index of line_tile, trig_tile, quad_tile
    # there will always be 4 line_tile
    # number of trig_tiles = 4(n - 2)
    # number of quad_tiles = n_tiles - 4(n - 2) - 4 
    
    
    
      
                          
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #------------------------------------ENCODE THE SHAPE INFO INTO ALL THE PARTICLES---------------------------------------
    """Fill the Shape Info in a np.array with shape (n**2, 4, 2) and randomize things a bit"""
        
     
    
    def tileCreator(pts, pos_vec, tile_netwrk):
        ShapeInfo = np.zeros((n_tiles, 4, 2), dtype = float)
        
        for tile in tile_netwrk:
            i = tile['tile_idx']
            if tile['tile_type'] == 'quad':
                ls = [[0,1], [1,2], [2,0], [3,3]]
            elif tile['tile_type'] == 'triHU':
                ls = [[0,1], [1,1], [2,0], [3,2]]
            elif tile['tile_type'] == 'triHL':
                ls = [[0,1], [1,2], [2,0], [3,0]] 
            elif tile['tile_type'] == 'triVL':
                ls = [[0,1], [1,0], [2,0], [3,2]]
            elif tile['tile_type'] == 'triVR':
                ls = [[0,0], [1,0], [2,1], [3,2]]
            elif tile['tile_type'] == 'lineLT':
                ls = [[0,0], [1,0], [2,1], [3,1]]
            elif tile['tile_type'] == 'lineRT':
                ls = [[0,1], [1,0], [2,0], [3,1]]    
            elif tile['tile_type'] == 'lineLB':
                ls = [[0,0], [1,1], [2,1], [3,0]]
            else:    
                ls = [[0,1], [1,1], [2,0], [3,0]]
                
            for j in ls:
                ShapeInfo[i, j[0], 0] = dist([pos_vec[3*(i+1)-3] - pts[tile['tile_vertices'][j[1]],0], 
                                          pos_vec[3*(i+1)-3 + 1] - pts[tile['tile_vertices'][j[1]],1]])
                
                ang = atan2(pts[tile['tile_vertices'][j[1]],1] - pos_vec[3*(i+1)-3 + 1], 
                                           pts[tile['tile_vertices'][j[1]],0] - pos_vec[3*(i+1)-3])
                ShapeInfo[i, j[0], 1] =  ang if ang > 0 else 2*np.pi + ang
        #print(ShapeInfo[0])           
        return(ShapeInfo)
                    
    #pts = pts
    ShapeInfo = tileCreator(pts, pos_vec, tile_netwrk)
                
    
                          
    #---------------------------------------ENCODE THE CONNECTION AMONG THE PARTICLES---------------------------------------
    '''ConnectedPolygonsMatrix(CPM) tells which two bodies are connected. The connections are first considered 'row- wise' 
    and than 'column-wise'. Every row of CPM denotes a connection.'''            
    cpm = np.zeros((n_conxn,2), dtype = int); j = 0
                                                                                                          
    for RowCxnNum in range(n):                                                                                                                
        for i in range (1 , n): 
            cpm[j] = [i + n*RowCxnNum-1, i + n*RowCxnNum + 1-1]
            j = j + 1
            
    for ColumnCxnNum in range(n):                                                                                                                
        for i in range (1 , n):
            cpm[j] = [ColumnCxnNum + n*i-(n-1)-1, ColumnCxnNum + n*i-(n-1) + n-1]
            j = j + 1        
    #-----------------------------------------------------------------------------------------------------------------------
    #CPM = np.delete(CPM, [5,11], 0)
    
    
    #--------------------------------------2D AND 3D BOOLEAN CONNECTED POLYGONS MATRIX--------------------------------------
    '''The connection among the polygons is represented in Boolean fashion. The number of rows in BooleanConnectedPolygonsM-
    trix2D(BCPM2D) = numberOfConnections. The number of column in the BooleanCPM is equal to len(PositionVector)'''
    bcpm2D =  np.zeros((n_conxn, len(pos_vec)), dtype = int)
    bcpm3D =  np.zeros((n_conxn, len(pos_vec), len(pos_vec)), dtype = int)
    
    for i in range(n_conxn):
        bcpm2D[i, 3*cpm[i,0]:3*cpm[i,0]+3] = 1  
        bcpm2D[i, 3*cpm[i,1]:3*cpm[i,1]+3] = 1
    
    for i in range(n_conxn):
        bcpm3D[i, 3*cpm[i,0]:3*cpm[i,0]+3, 3*cpm[i,0]:3*cpm[i,0]+3] = 1  
        bcpm3D[i, 3*cpm[i,1]:3*cpm[i,1]+3, 3*cpm[i,1]:3*cpm[i,1]+3] = 1 
        bcpm3D[i, 3*cpm[i,0]:3*cpm[i,0]+3, 3*cpm[i,1]:3*cpm[i,1]+3] = 1
        bcpm3D[i, 3*cpm[i,1]:3*cpm[i,1]+3, 3*cpm[i,0]:3*cpm[i,0]+3] = 1
        
    #-----------------------------------------------------------------------------------------------------------------------                                   
                                       
    
    
    #--------------------------------ENCODE WHICH CORNERS ARE CONNECTED IN A CONNECTION-------------------------------------
    '''A row of ConnectedCornersMatrix(CCM) tells the Corner Number of the Polygon that share the Connection'''
    ccm = np.zeros((n_conxn,2), dtype = int)        
    ccm[0:int(n_conxn/2)]=[0,2]
    ccm[int(n_conxn/2):n_conxn]=[3,1]
    #ccm[0:5] = [0,2]
    #ccm[5:11] = [3,1]
    #ccm[11:16] = [0,2]
    #ccm[16:22] = [3,1]
    #ccm[22:27] = [0,2]
    #ccm[27:33] = [3,1]
    #ccm[33:38] = [0,2]
    #ccm[38:44] = [3,1]
    #ccm[44:49] = [0,2]
    #ccm[49:55] = [3,1]
    #ccm[55:60] = [0,2]
    #-----------------------------------------------------------------------------------------------------------------------
    #CCM = np.delete(CCM, [5,11], 0)
    
    
    
    
    #theta  = conc((lnsp(0, 0.5,5).reshape(5,1),lnsp(0, -0.5, 5).reshape(5,1)))
    #theta  = lnsp(-1.04, 1.04, 10).reshape(10,1)    
    theta = [0]
    #theta  = lnsp(1.04, -1.04, 100).reshape(100,1)    
    #theta = np.array([0])
    thetaCount     = 0
    FinalEnergy    = np.zeros([len(theta), 1])
    Distance       = np.zeros([len(theta), 1])
    PositionVector = pos_vec      
    
    #FigNumber = 0; plt.figure(FigNumber); FigNumber += 1
    #visualize(PositionVector, n, ShapeInfo)
    
    fig_num = 0
    
    for PositionVector[14] in theta:    #44 for 6x6
        
        #print('*************\n**********\n************\n***************', PositionVector[5])
        
        PositionVector = pos_vec if PositionVector[14] == 0.0 else PositionVector
            
        #---------------------------------X1, Y1, Theta1, L1, Phi1, X2, Y2, Theta2, L2, Phi2--------------------------------
        ''' The 10 required vectors for Vectorization'''
        X1 = np.zeros((n_conxn,));Y1 = np.copy(X1);Theta1 = np.copy(X1);LenVec1 =np.copy(X1);Phi1=np.copy(X1)
        X2 = np.zeros((n_conxn,));Y2 = np.copy(X1);Theta2 = np.copy(X1);LenVec2 =np.copy(X1);Phi2=np.copy(X1)
        
        for i in range(n_conxn):
            X1[i]       = PositionVector[3*cpm[i, 0], 0]   
            Y1[i]       = PositionVector[3*cpm[i, 0] + 1, 0]
            Theta1[i]   = PositionVector[3*cpm[i, 0] + 2, 0]
            LenVec1[i]  = ShapeInfo[cpm[i, 0], ccm[i, 0],0]
            Phi1[i]     = ShapeInfo[cpm[i, 0], ccm[i, 0],1]
            
            X2[i]       = PositionVector[3*cpm[i, 1], 0]
            Y2[i]       = PositionVector[3*cpm[i, 1] + 1, 0]
            Theta2[i]   = PositionVector[3*cpm[i, 1] + 2, 0]
            LenVec2[i]  = ShapeInfo[cpm[i, 1], ccm[i, 1],0]
            Phi2[i]     = ShapeInfo[cpm[i, 1], ccm[i, 1],1]
        #-------------------------------------------------------------------------------------------------------------------
            
           
        #----------------------NON LINEAR CONJUGATE GRADIENT ALGORITHM WITH NEWTON RAPHSON AND FLETCHER REEVS---------------
        '''Conjugate gradient algorithm to search for the equilibrium state of the mechanism for a given configuration''' 
        
        j = 0 ; j_max = 1000 ; k = 0 
        
        
        Network = UpdateNetwork(n_tiles, X1, Y1, Theta1, LenVec1, Phi1, X2, Y2, Theta2, LenVec2, Phi2, bcpm2D, bcpm3D)
        
        FirstDerivativeVector  = Network.UpdateFirstDerivativeVector()
        SecondDerivativeMatrix = Network.UpdateSecondDerivativeMatrix()
        
        EnergyInSprings        = np.zeros([j_max,1])
        Iterations             = np.zeros([j_max,1])
        EnergyInSprings[j, 0]  = Network.UpdateEnergyInSprings()
        Iterations[j,0]        = j   
        
        
        r       =  -FirstDerivativeVector
        d       =  r
        del_new =  dotProduct(transpose(r), r)
        #del_o   =  del_new
        #t0 = time.time()
        
        #if False:
        while (j < j_max):
            #print(j)
            
            #del_d = dotProduct(transpose(d), d)
            alpha = -(dotProduct(transpose(FirstDerivativeVector), d))/  \
                     (dotProduct(transpose(d),dotProduct(SecondDerivativeMatrix,d)))
            
            PositionVector = PositionVector + alpha*d
            
            #-------------------------------X1, Y1, Theta1, L1, Phi1, X2, Y2, Theta2, L2, Phi2------------------------------
            ''' The 10 required vectors for Vectorization'''        
            for i in range(n_conxn):
                X1[i]       = PositionVector[3*cpm[i, 0], 0]   
                Y1[i]       = PositionVector[3*cpm[i, 0] + 1, 0]
                Theta1[i]   = PositionVector[3*cpm[i, 0] + 2, 0]
                LenVec1[i]  = ShapeInfo[cpm[i, 0], ccm[i, 0],0]
                Phi1[i]     = ShapeInfo[cpm[i, 0], ccm[i, 0],1]
                
                X2[i]       = PositionVector[3*cpm[i, 1], 0]
                Y2[i]       = PositionVector[3*cpm[i, 1] + 1, 0]
                Theta2[i]   = PositionVector[3*cpm[i, 1] + 2, 0]
                LenVec2[i]  = ShapeInfo[cpm[i, 1], ccm[i, 1],0]
                Phi2[i]     = ShapeInfo[cpm[i, 1], ccm[i, 1],1]
            #---------------------------------------------------------------------------------------------------------------
                
            
            Network = UpdateNetwork(n_tiles, X1, Y1, Theta1, LenVec1, Phi1, X2, Y2, Theta2, LenVec2, Phi2, bcpm2D, bcpm3D)
            
            FirstDerivativeVector      = Network.UpdateFirstDerivativeVector()
            SecondDerivativeMatrix     = Network.UpdateSecondDerivativeMatrix()
            
            if(j < j_max - 1):
                EnergyInSprings[j + 1, 0]  = Network.UpdateEnergyInSprings()
                Iterations[j + 1,0]        = j + 1
        
            
            #t2 = time.time()
            r       = -FirstDerivativeVector
            del_old = del_new
            del_new = dotProduct(transpose(r), r)
            beta    = del_new/del_old
            d       = r + beta*d
    
            
            #------------------LOOP RESTART------------------------
            k = k + 1
            if (k==24) or (dotProduct(transpose(r), d) <= 0):
                #print('*******restarted')
                d = r
                k = 0
            #------------------------------------------------------
            #t3 = time.time()  
              
            #------------BREAK OUT IF CONVERGENCE------------------ 
            E1  =  log10(EnergyInSprings[EnergyInSprings != 0][-1])  
            E2  =  log10(EnergyInSprings[EnergyInSprings != 0][-2])
            if (  E1   < -10 or E2 - E1 < 0.00001):
                break
            #------------------------------------------------------
            j = j + 1  
             
        #-------------------------------------------------------------------------------------------------------------------      
         
        print(PositionVector[14], EnergyInSprings[EnergyInSprings != 0][-1], j)
        #plt.plot(PositionVector[44], EnergyInSprings[EnergyInSprings != 0][-1], 'o', c='r')
        
        #print(PositionVector[5], EnergyInSprings[EnergyInSprings != 0][-1], j)
        #plt.plot(PositionVector[5], EnergyInSprings[EnergyInSprings != 0][-1], 'o', c='r')
        
        plt.figure()
        visualize(PositionVector, n, ShapeInfo)
        plt.savefig(r'C:\Users\Nitin.N.Singh\Desktop\RAUX\Python\6x6\Figures\Movie\fig{0}.png'.format(fig_num))
        #fig_num += 1
        #plt.close()
        #----------------------------------------FINAL INFORMATION OF THE TILING--------------------------------------------
        FinalEnergy[thetaCount, 0] = E1
        #Distance[thetaCount, 0] = sqrt(((ShapeInfo[5,2,0]*cos(ShapeInfo[5,2,1]+PositionVector[17,0])+PositionVector[15,0])-\
        #        (ShapeInfo[7,3,0]*cos(ShapeInfo[7,3,1]+PositionVector[23,0])+PositionVector[21,0]))**2+((ShapeInfo[5,2,0]* \
        #        sin(ShapeInfo[5,2,1]+PositionVector[17,0])+ PositionVector[16,0])-(ShapeInfo[7,3,0]*sin(ShapeInfo[7,3,1]+  \
        #        PositionVector[23,0])+PositionVector[22,0]))**2)
        thetaCount += 1               
        #------------------------------------------------------------------------------------------------------------------- 
    #FigNumber += 1    
        
    #Distance[0:50] = np.flipud(Distance[0:50])    
    #theta[0:50] = np.flipud(theta[0:50])
         
    
    #-----------------------------------------------DISTANCE CURVE----------------------------------------------------------
    #plt.figure(FigNumber); FigNumber += 1
    print('>>>>', FinalEnergy)
    plt.plot(theta*180/np.pi, FinalEnergy, 'o')
    #plt.ylim((0.5,1.5)); plt.xlim((-75,75))
    
    #plt.xticks([-60, 0, 60], fontsize = 10); plt.yticks([8.0, 6.0,  4.0, 2.0, 0.0], fontsize = 10)  
    #plt.xticks([-60, 0, 60]); plt.yticks([0.50,  1.0, 1.50])  
    #plt.grid(True)
    #-----------------------------------------------------------------------------------------------------------------------                    
    
    t1 = time.time()
    
    print('Code took ', t1 - t0 , ' time to run')
    
    r'''
    pts = np.zeros((n_conxn,2), dtype = float)
    pts[0] = [-(n/2 - 1), n/2-0.5]
    
    for j in range(n-1):
        for i in range(1,n-1):
            pts[i + j*(2*n-1)] = pts[i + j*(2*n-1)-1] + [1,0]
        for i in range(n-1,2*n-2):
            pts[i + j*(2*n-1)] = pts[i + j*(2*n-1)-5] - [0.5,0.5]
     
    plt.plot(pts[:,0], pts[:,1])
    plt.plot(pts[:,0], pts[:,1], 'o')       
    '''
    
main_func()    









