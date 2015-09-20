# P442 Local targets

download: $(BUILDDIR)/$(PROJECT).bin                                            
	st-flash write $(BUILDDIR)/$(PROJECT).bin 0x8000000 > st-flash.log 2>&1    

etags:                                                                          
	find $(SRCPATHS) -type f -iname "*.[ch]" | xargs etags --append         
	find $(INCDIR) -type f -iname "*.[ch]" | xargs etags --append           
	find $(DINCDIR) -type f -iname "*.[ch]" | xargs etags --append          
	find $(UINCDIR) -type f -iname "*.[ch]" | xargs etags --append 
