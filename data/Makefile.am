include $(top_srcdir)/common.am

dlnaschemas = dlna-profiles.rng
dlnaprofiles = mp3.xml \
	       ac3.xml \
	       lpcm.xml \
	       aac.xml \
	       wma.xml \
	       amr.xml \
	       common.xml \
	       mpeg1.xml \
	       avc.xml \
	       mpeg-ts.xml \
	       mpeg-ps.xml \
	       mpeg-common.xml \
	       mpeg4.xml \
	       jpeg.xml \
	       png.xml

dlnadir = $(shareddir)/dlna-profiles
dlna_DATA = $(dlnaschemas) $(dlnaprofiles)

if HAVE_XMLLINT
check-profiles: $(dlnaprofiles) $(dlnaschemas)
	$(XMLLINT) --relaxng $(dlnaschemas) $(dlnaprofiles) >/dev/null
endif

EXTRA_DIST = $(dlnaschemas) $(dlnaprofiles)
