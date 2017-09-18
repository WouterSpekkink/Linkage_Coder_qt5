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
    "fig:importoptions"
    "sec:importerrors"
    "fig:importerror"
    "sec:savingloadingdata"
    "fig:saveload"
    "sec:importingcodes"
    "fig:importcodesfig"
    "fig:importingcodeserror"
    "fig:importingcodesdiagram"
    "sec:problemsimportingcodes"
    "sec:startinganewsession"
    "fig:settings"
    "sec:navigatingdata"
    "fig:incidentsoverview"
    "fig:extrafields"
    "fig:indexes"
    "sec:markingincidents"
    "fig:flagoptions"
    "fig:flaggedincident"
    "sec:codinglinkages"
    "fig:linkage"
    "fig:memo"
    "sec:exportingdata"
    "fig:exportdialog"
    "sec:logfilesandexiting"
    "sec:comparingcodes"
    "fig:codecomparisondialog"
    "fig:comparedcodes"
    "fig:screenshotresults"
    "chap:whatisnext"
    "fig:eventgraphone"
    "sec:graphdatabasesandeventgraphs"
    "fig:graphdatabaseexample"
    "fig:expandedgraphdatabase"
    "fig:exportingneo4j"
    "fig:eventgraphtwo"
    "chap:contactdetails"))
 :latex)

