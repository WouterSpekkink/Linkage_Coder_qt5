(TeX-add-style-hook
 "Linkage_Coder_Manual"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("fontenc" "T1")))
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperref")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (TeX-run-style-hooks
    "latex2e"
    "memoir"
    "memoir10"
    "xcolor"
    "hyperref"
    "graphicx"
    "enumitem"
    "verbatim"
    "framed"
    "fontenc")
   (LaTeX-add-labels
    "chap:introduction"
    "sec:eventcoderprogram"
    "sec:partofbiggerprogram"
    "sec:disclaimers"
    "chap:preparations"
    "sec:allaboutdata"
    "sec:datasets"
    "fig:datasetstructure"
    "fig:csvfile"
    "sec:othernotesdatasets"
    "chap:usingtheprogram"
    "sec:loadingnewdataset"
    "sec:importerrors"
    "fig:importerror"
    "sec:savingloadingdata"
    "sec:importingcodes"
    "fig:importingcodesdiagram"
    "sec:problemsimportingcodes"
    "fig:overwritingcodes"
    "sec:navigatingdata"
    "sec:markingincidents"
    "sec:exportingdata"
    "sec:logfilesandexiting"
    "chap:usingtheprogram2"
    "sec:introductiontocoding"
    "chap:whatisnext"
    "sec:graphdatabases"
    "sec:creatingeventgraphs"
    "chap:contactdetails"))
 :latex)

