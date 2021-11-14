# -*- coding: utf-8 -*-
"""
Created on Sun Feb 14 09:18:32 2021

@author: Nitin.N.Singh
"""

from bing_image_downloader import downloader
downloader.download('mechanical metamaterials', limit=100,  output_dir= 'Images', 
adult_filter_off=True, force_replace=False, timeout=60)