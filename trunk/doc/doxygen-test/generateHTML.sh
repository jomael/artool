#!/bin/sh

HTML_OUT_PATH=../html
HTML_IN_PATH=./templates
IMG_PATH=./images

concatenate()
{
	tmpFile=$$;
	cat ${HTML_IN_PATH}/$1 ${HTML_IN_PATH}/$2 ${HTML_OUT_PATH}/$3 > ${tmpFile};
	mv ${tmpFile} ${HTML_OUT_PATH}/$3;
}



doxygen doc_html.conf;

#cp ${IMG_PATH}/mein_* ${HTML_OUT_PATH}

#concatenate hdr.html mnu_index.html index.html;
#concatenate hdr.html mnu_act.html sp__a_c_t.html;

exit 0;


