.PHONY: all visualize run test lib clean

TARGETPATH := ./bin

all: lib test run visualize

visualize: lib
	@$(MAKE) --no-print-directory -C Support/VisualizePso
	@cp ./Support/VisualizePso/VisualizePso $(TARGETPATH)

run: lib
	@$(MAKE) --no-print-directory -C Support/RunPso
	@cp ./Support/RunPso/RunPso $(TARGETPATH)

test: lib
	@$(MAKE) --no-print-directory -C UnitTest/TestPso
	@cp ./UnitTest/TestPso/TestPso $(TARGETPATH)

lib:
	@mkdir -p $(TARGETPATH)
	@$(MAKE) --no-print-directory -C Pso
	@cp ./UnitTest/TestData/default50 $(TARGETPATH)
	@cp ./UnitTest/TestData/default10 $(TARGETPATH)
	@cp ./UnitTest/TestData/default2 $(TARGETPATH)
	@cp ./Pso/libPso.so $(TARGETPATH)

clean:
	@$(MAKE) --no-print-directory -C Support/VisualizePso $@
	@$(MAKE) --no-print-directory -C Support/RunPso $@
	@$(MAKE) --no-print-directory -C UnitTest/TestPso $@
	@$(MAKE) --no-print-directory -C Pso $@
	@$(RM) -rf $(TARGETPATH)
