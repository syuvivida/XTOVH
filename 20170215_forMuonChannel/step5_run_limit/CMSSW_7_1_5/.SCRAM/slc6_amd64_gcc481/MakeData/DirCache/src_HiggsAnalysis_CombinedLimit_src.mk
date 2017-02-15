ifeq ($(strip $(HiggsAnalysis/CombinedLimit)),)
ALL_COMMONRULES += src_HiggsAnalysis_CombinedLimit_src
src_HiggsAnalysis_CombinedLimit_src_parent := HiggsAnalysis/CombinedLimit
src_HiggsAnalysis_CombinedLimit_src_INIT_FUNC := $$(eval $$(call CommonProductRules,src_HiggsAnalysis_CombinedLimit_src,src/HiggsAnalysis/CombinedLimit/src,LIBRARY))
HiggsAnalysisCombinedLimit := self/HiggsAnalysis/CombinedLimit
HiggsAnalysis/CombinedLimit := HiggsAnalysisCombinedLimit
HiggsAnalysisCombinedLimit_files := $(patsubst src/HiggsAnalysis/CombinedLimit/src/%,%,$(wildcard $(foreach dir,src/HiggsAnalysis/CombinedLimit/src ,$(foreach ext,$(SRC_FILES_SUFFIXES),$(dir)/*.$(ext)))))
HiggsAnalysisCombinedLimit_BuildFile    := $(WORKINGDIR)/cache/bf/src/HiggsAnalysis/CombinedLimit/BuildFile
HiggsAnalysisCombinedLimit_LOC_LIB   := Smatrix
HiggsAnalysisCombinedLimit_LOC_USE := self  root rootmath roofit roostats histfactory libxml2 vdt boost_program_options boost_filesystem
HiggsAnalysisCombinedLimit_ROOTDICT  := LinkDef
HiggsAnalysisCombinedLimit_PRE_INIT_FUNC += $$(eval $$(call RootDict,HiggsAnalysisCombinedLimit,src/HiggsAnalysis/CombinedLimit/src,CombinedLimit_LinkDef.h,$(SCRAMSTORENAME_LIB)))
HiggsAnalysisCombinedLimit_EX_LIB   := HiggsAnalysisCombinedLimit
HiggsAnalysisCombinedLimit_EX_USE   := $(foreach d,$(HiggsAnalysisCombinedLimit_LOC_USE),$(if $($(d)_EX_FLAGS_NO_RECURSIVE_EXPORT),,$d))
HiggsAnalysisCombinedLimit_PACKAGE := self/src/HiggsAnalysis/CombinedLimit/src
ALL_PRODS += HiggsAnalysisCombinedLimit
HiggsAnalysisCombinedLimit_CLASS := LIBRARY
HiggsAnalysis/CombinedLimit_forbigobj+=HiggsAnalysisCombinedLimit
HiggsAnalysisCombinedLimit_INIT_FUNC        += $$(eval $$(call Library,HiggsAnalysisCombinedLimit,src/HiggsAnalysis/CombinedLimit/src,src_HiggsAnalysis_CombinedLimit_src,$(SCRAMSTORENAME_BIN),,$(SCRAMSTORENAME_LIB),$(SCRAMSTORENAME_LOGS)))
endif
