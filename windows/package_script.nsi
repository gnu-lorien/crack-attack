; Script generated with the Venis Install Wizard

;Debug Informations
; !define DEBUG

; Define your application name
!define APPNAME "Crack-Attack!"
!define APPNAMEANDVERSION "Crack-Attack! 1.1.14"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\Crack-Attack"
OutFile "${APPNAMEANDVERSION} Setup.exe"

; Use compression
SetCompressor BZip2

; Modern interface settings
!include "MUI.nsh"

!define MUI_ICON "cube_icon.ico"
!define MUI_UNICON "cube_icon.ico"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "Crack-Attack\COPYING.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Var isadmin
Var winver

!macro checkadmin
	;Check whether admin or not

	ClearErrors
	UserInfo::GetAccountType
	IfErrors admin ;Win 9x
	Pop $1
	StrCmp $1 "Admin" admin
	Goto user ;Error or not admin -> user_inst

	admin:
		!ifdef DEBUG
			MessageBox MB_OK 'Admin install'
		!endif
		Push 1
		goto ahead
	user:
		!ifdef DEBUG
			MessageBox MB_OK 'User install'
		!endif
		Push 0
	ahead:
		Pop $isadmin

!macroend

 ; GetWindowsVersion
 ;
 ; Based on Yazno's function, http://yazno.tripod.com/powerpimpit/
 ; Updated by Joost Verburg
 ;
 ; Returns on top of stack
 ;
 ; Windows Version (95, 98, ME, NT x.x, 2000, XP, 2003)
 ; or
 ; '' (Unknown Windows Version)
 ;
 ; Usage:
 ;   Call GetWindowsVersion
 ;   Pop $R0
 ;   ; at this point $R0 is "NT 4.0" or whatnot
 
 Function GetWindowsVersion
 
   Push $R0
   Push $R1
 
   ClearErrors
 
   ReadRegStr $R0 HKLM \
   "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion

   IfErrors 0 lbl_winnt
   
   ; we are not NT
   ReadRegStr $R0 HKLM \
   "SOFTWARE\Microsoft\Windows\CurrentVersion" VersionNumber
 
   StrCpy $R1 $R0 1
   StrCmp $R1 '4' 0 lbl_error
 
   StrCpy $R1 $R0 3
 
   StrCmp $R1 '4.0' lbl_win32_95
   StrCmp $R1 '4.9' lbl_win32_ME lbl_win32_98
 
   lbl_win32_95:
     StrCpy $R0 '95'
   Goto lbl_done
 
   lbl_win32_98:
     StrCpy $R0 '98'
   Goto lbl_done
 
   lbl_win32_ME:
     StrCpy $R0 'ME'
   Goto lbl_done
 
   lbl_winnt:
 
   StrCpy $R1 $R0 1
 
   StrCmp $R1 '3' lbl_winnt_x
   StrCmp $R1 '4' lbl_winnt_x
 
   StrCpy $R1 $R0 3
 
   StrCmp $R1 '5.0' lbl_winnt_2000
   StrCmp $R1 '5.1' lbl_winnt_XP
   StrCmp $R1 '5.2' lbl_winnt_2003 lbl_error
 
   lbl_winnt_x:
     StrCpy $R0 "NT $R0" 6
   Goto lbl_done
 
   lbl_winnt_2000:
     Strcpy $R0 '2000'
   Goto lbl_done
 
   lbl_winnt_XP:
     Strcpy $R0 'XP'
   Goto lbl_done
 
   lbl_winnt_2003:
     Strcpy $R0 '2003'
   Goto lbl_done
 
   lbl_error:
     Strcpy $R0 ''
   lbl_done:
 
   Pop $R1
   Exch $R0
 
FunctionEnd

Function .onInit
	!insertmacro checkadmin
	Call GetWindowsVersion
	Pop $winver
FunctionEnd

Function un.onInit
	!insertmacro checkadmin
FunctionEnd

Section "Crack-Attack!" Section1

	;Check whether we should install Crack-Attack! for all users or just for current user
	StrCmp $winver "95" user_inst
	StrCmp $winver "98" user_inst
	StrCmp $winver "ME" user_inst ;For these windows versions: user install
	
	IntCmp $isadmin 1 admin_inst user_inst
admin_inst:
	MessageBox MB_YESNO "Do you want to install ${APPNAME} for all users?" IDYES all_yes IDNO user_inst
all_yes:
	SetShellVarContext all
	goto ahead_inst
user_inst:
	SetShellVarContext current
ahead_inst:

	; Set Section properties
	SetOverwrite on

	Delete "$INSTDIR\uninst.exe"

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR\"
	File "Crack-Attack\COPYING.LIB-2"
	File "Crack-Attack\README.libiconv"
	File "Crack-Attack\README.txt"
	File "Crack-Attack\COPYING.txt"
	SetOutPath "$INSTDIR\bin\"
	File "Crack-Attack\bin\gettext.sh"
	File "Crack-Attack\bin\crack-attack.exe"
	File "Crack-Attack\bin\crack-attack-gui.exe"
	File "Crack-Attack\bin\envsubst.exe"
	File "Crack-Attack\bin\gettext.exe"
	File "Crack-Attack\bin\iconv.exe"
	File "Crack-Attack\bin\ngettext.exe"
	File "Crack-Attack\bin\asprintf.dll"
	File "Crack-Attack\bin\charset.dll"
	File "Crack-Attack\bin\freetype6.dll"
	File "Crack-Attack\bin\gspawn-win32-helper.exe"
	File "Crack-Attack\bin\iconv.dll"
	File "Crack-Attack\bin\intl.dll"
	File "Crack-Attack\bin\jpeg62.dll"
	File "Crack-Attack\bin\libatk-1.0-0.dll"
	File "Crack-Attack\bin\libexpat-0.dll"
	File "Crack-Attack\bin\libfontconfig-1.dll"
	File "Crack-Attack\bin\libgdk-win32-2.0-0.dll"
	File "Crack-Attack\bin\libgdk_pixbuf-2.0-0.dll"
	File "Crack-Attack\bin\libglib-2.0-0.dll"
	File "Crack-Attack\bin\libgmodule-2.0-0.dll"
	File "Crack-Attack\bin\libgobject-2.0-0.dll"
	File "Crack-Attack\bin\libgthread-2.0-0.dll"
	File "Crack-Attack\bin\libgtk-win32-2.0-0.dll"
	File "Crack-Attack\bin\libpango-1.0-0.dll"
	File "Crack-Attack\bin\libpangoft2-1.0-0.dll"
	File "Crack-Attack\bin\libpangowin32-1.0-0.dll"
	File "Crack-Attack\bin\libpng12.dll"
	File "Crack-Attack\bin\libpng13.dll"
	File "Crack-Attack\bin\libtiff3.dll"
	File "Crack-Attack\bin\pango-querymodules.exe"
	File "Crack-Attack\bin\xmlparse.dll"
	File "Crack-Attack\bin\xmltok.dll"
	File "Crack-Attack\bin\zlib1.dll"
	SetOutPath "$INSTDIR\data\"
	File "Crack-Attack\data\default_multiplier"
	File "Crack-Attack\data\default_record"
	File "Crack-Attack\data\Makefile.am"
	File "Crack-Attack\data\clock_0.tga"
	File "Crack-Attack\data\clock_1.tga"
	File "Crack-Attack\data\clock_2.tga"
	File "Crack-Attack\data\clock_3.tga"
	File "Crack-Attack\data\clock_4.tga"
	File "Crack-Attack\data\clock_5.tga"
	File "Crack-Attack\data\clock_6.tga"
	File "Crack-Attack\data\clock_7.tga"
	File "Crack-Attack\data\clock_8.tga"
	File "Crack-Attack\data\clock_9.tga"
	File "Crack-Attack\data\clock_extra.tga"
	File "Crack-Attack\data\count_down_1.png"
	File "Crack-Attack\data\count_down_2.png"
	File "Crack-Attack\data\count_down_3.png"
	File "Crack-Attack\data\count_down_go.png"
	File "Crack-Attack\data\flavor_1.png"
	File "Crack-Attack\data\flavor_2.png"
	File "Crack-Attack\data\font0_0.tga"
	File "Crack-Attack\data\font0_1.tga"
	File "Crack-Attack\data\font0_2.tga"
	File "Crack-Attack\data\font0_3.tga"
	File "Crack-Attack\data\font0_4.tga"
	File "Crack-Attack\data\font0_5.tga"
	File "Crack-Attack\data\font0_6.tga"
	File "Crack-Attack\data\font0_7.tga"
	File "Crack-Attack\data\font0_8.tga"
	File "Crack-Attack\data\font0_9.tga"
	File "Crack-Attack\data\font0_a.tga"
	File "Crack-Attack\data\font0_and.tga"
	File "Crack-Attack\data\font0_at.tga"
	File "Crack-Attack\data\font0_b.tga"
	File "Crack-Attack\data\font0_br.tga"
	File "Crack-Attack\data\font0_c.tga"
	File "Crack-Attack\data\font0_ca.tga"
	File "Crack-Attack\data\font0_cb.tga"
	File "Crack-Attack\data\font0_cc.tga"
	File "Crack-Attack\data\font0_cd.tga"
	File "Crack-Attack\data\font0_ce.tga"
	File "Crack-Attack\data\font0_cf.tga"
	File "Crack-Attack\data\font0_cg.tga"
	File "Crack-Attack\data\font0_ch.tga"
	File "Crack-Attack\data\font0_ci.tga"
	File "Crack-Attack\data\font0_cj.tga"
	File "Crack-Attack\data\font0_ck.tga"
	File "Crack-Attack\data\font0_cl.tga"
	File "Crack-Attack\data\font0_cln.tga"
	File "Crack-Attack\data\font0_cm.tga"
	File "Crack-Attack\data\font0_cma.tga"
	File "Crack-Attack\data\font0_cn.tga"
	File "Crack-Attack\data\font0_co.tga"
	File "Crack-Attack\data\font0_cp.tga"
	File "Crack-Attack\data\font0_cq.tga"
	File "Crack-Attack\data\font0_cr.tga"
	File "Crack-Attack\data\font0_cs.tga"
	File "Crack-Attack\data\font0_ct.tga"
	File "Crack-Attack\data\font0_cu.tga"
	File "Crack-Attack\data\font0_cv.tga"
	File "Crack-Attack\data\font0_cw.tga"
	File "Crack-Attack\data\font0_cx.tga"
	File "Crack-Attack\data\font0_cy.tga"
	File "Crack-Attack\data\font0_cz.tga"
	File "Crack-Attack\data\font0_d.tga"
	File "Crack-Attack\data\font0_ds.tga"
	File "Crack-Attack\data\font0_e.tga"
	File "Crack-Attack\data\font0_ep.tga"
	File "Crack-Attack\data\font0_eq.tga"
	File "Crack-Attack\data\font0_f.tga"
	File "Crack-Attack\data\font0_g.tga"
	File "Crack-Attack\data\font0_gt.tga"
	File "Crack-Attack\data\font0_h.tga"
	File "Crack-Attack\data\font0_i.tga"
	File "Crack-Attack\data\font0_j.tga"
	File "Crack-Attack\data\font0_k.tga"
	File "Crack-Attack\data\font0_l.tga"
	File "Crack-Attack\data\font0_lt.tga"
	File "Crack-Attack\data\font0_m.tga"
	File "Crack-Attack\data\font0_mn.tga"
	File "Crack-Attack\data\font0_n.tga"
	File "Crack-Attack\data\font0_o.tga"
	File "Crack-Attack\data\font0_p.tga"
	File "Crack-Attack\data\font0_pd.tga"
	File "Crack-Attack\data\font0_pe.tga"
	File "Crack-Attack\data\font0_pl.tga"
	File "Crack-Attack\data\font0_pr.tga"
	File "Crack-Attack\data\font0_ps.tga"
	File "Crack-Attack\data\font0_pu.tga"
	File "Crack-Attack\data\font0_q.tga"
	File "Crack-Attack\data\font0_qm.tga"
	File "Crack-Attack\data\font0_r.tga"
	File "Crack-Attack\data\font0_s.tga"
	File "Crack-Attack\data\font0_sl.tga"
	File "Crack-Attack\data\font0_t.tga"
	File "Crack-Attack\data\font0_td.tga"
	File "Crack-Attack\data\font0_u.tga"
	File "Crack-Attack\data\font0_v.tga"
	File "Crack-Attack\data\font0_w.tga"
	File "Crack-Attack\data\font0_x.tga"
	File "Crack-Attack\data\font0_y.tga"
	File "Crack-Attack\data\font0_z.tga"
	File "Crack-Attack\data\font0_za.tga"
	File "Crack-Attack\data\font0_zb.tga"
	File "Crack-Attack\data\font0_zc.tga"
	File "Crack-Attack\data\font0_zd.tga"
	File "Crack-Attack\data\garbage_flavor_000.png"
	File "Crack-Attack\data\garbage_flavor_001.png"
	File "Crack-Attack\data\garbage_flavor_002.png"
	File "Crack-Attack\data\garbage_flavor_003.png"
	File "Crack-Attack\data\garbage_flavor_004.png"
	File "Crack-Attack\data\garbage_flavor_005.png"
	File "Crack-Attack\data\garbage_flavor_logo.png"
	File "Crack-Attack\data\garbage_flavor_logo_x.png"
	File "Crack-Attack\data\garbage_lightmap_0.png"
	File "Crack-Attack\data\garbage_lightmap_1.png"
	File "Crack-Attack\data\garbage_lightmap_2.png"
	File "Crack-Attack\data\garbage_lightmap_3.png"
	File "Crack-Attack\data\garbage_lightmap_4.png"
	File "Crack-Attack\data\garbage_lightmap_5.png"
	File "Crack-Attack\data\logo.png"
	File "Crack-Attack\data\logo_x.png"
	File "Crack-Attack\data\message_anykey.png"
	File "Crack-Attack\data\message_game_over.png"
	File "Crack-Attack\data\message_loser.png"
	File "Crack-Attack\data\message_paused.png"
	File "Crack-Attack\data\message_waiting.png"
	File "Crack-Attack\data\message_winner.png"
	File "Crack-Attack\data\sign_10.tga"
	File "Crack-Attack\data\sign_11.tga"
	File "Crack-Attack\data\sign_12.tga"
	File "Crack-Attack\data\sign_4.tga"
	File "Crack-Attack\data\sign_5.tga"
	File "Crack-Attack\data\sign_6.tga"
	File "Crack-Attack\data\sign_7.tga"
	File "Crack-Attack\data\sign_8.tga"
	File "Crack-Attack\data\sign_9.tga"
	File "Crack-Attack\data\sign_bonus.tga"
	File "Crack-Attack\data\sign_x10.tga"
	File "Crack-Attack\data\sign_x11.tga"
	File "Crack-Attack\data\sign_x12.tga"
	File "Crack-Attack\data\sign_x2.tga"
	File "Crack-Attack\data\sign_x3.tga"
	File "Crack-Attack\data\sign_x4.tga"
	File "Crack-Attack\data\sign_x5.tga"
	File "Crack-Attack\data\sign_x6.tga"
	File "Crack-Attack\data\sign_x7.tga"
	File "Crack-Attack\data\sign_x8.tga"
	File "Crack-Attack\data\sign_x9.tga"
	File "Crack-Attack\data\preview_reduced.png"
	File "Crack-Attack\data\preview_normal.png"
	File "Crack-Attack\data\preview_extremely_reduced.png"
	File "Crack-Attack\data\crack-attack.xpm"
	SetOutPath "$INSTDIR\etc\pango\"
	File "Crack-Attack\etc\pango\pango.aliases"
	File "Crack-Attack\etc\pango\pango.modules"
	SetOutPath "$INSTDIR\etc\gtk-2.0\"
	File "Crack-Attack\etc\gtk-2.0\gdk-pixbuf.loaders"
	File "Crack-Attack\etc\gtk-2.0\gtk.immodules"
	File "Crack-Attack\etc\gtk-2.0\gtkrc"
	SetOutPath "$INSTDIR\etc\fonts\"
	File "Crack-Attack\etc\fonts\fonts.conf"
	File "Crack-Attack\etc\fonts\local.conf"
	SetOutPath "$INSTDIR\include\"
	File "Crack-Attack\include\autosprintf.h"
	File "Crack-Attack\include\iconv.h"
	File "Crack-Attack\include\libcharset.h"
	File "Crack-Attack\include\libintl.h"
	File "Crack-Attack\include\localcharset.h"
	SetOutPath "$INSTDIR\lib\"
	File "Crack-Attack\lib\libiconv.a"
	File "Crack-Attack\lib\libintl.a"
	File "Crack-Attack\lib\asprintf.lib"
	File "Crack-Attack\lib\charset.lib"
	File "Crack-Attack\lib\iconv.lib"
	File "Crack-Attack\lib\intl.lib"
	SetOutPath "$INSTDIR\lib\pango\1.4.0\modules\"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-arabic-fc.dll"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-basic-fc.dll"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-basic-win32.dll"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-hangul-fc.dll"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-hebrew-fc.dll"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-indic-fc.dll"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-syriac-fc.dll"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-thai-fc.dll"
	File "Crack-Attack\lib\pango\1.4.0\modules\pango-tibetan-fc.dll"
	SetOutPath "$INSTDIR\lib\locale\en_GB\LC_MESSAGES\"
	File "Crack-Attack\lib\locale\en_GB\LC_MESSAGES\atk10.mo"
	File "Crack-Attack\lib\locale\en_GB\LC_MESSAGES\glib20.mo"
	File "Crack-Attack\lib\locale\en_GB\LC_MESSAGES\gtk20.mo"
	SetOutPath "$INSTDIR\lib\locale\en_CA\LC_MESSAGES\"
	File "Crack-Attack\lib\locale\en_CA\LC_MESSAGES\atk10.mo"
	File "Crack-Attack\lib\locale\en_CA\LC_MESSAGES\glib20.mo"
	File "Crack-Attack\lib\locale\en_CA\LC_MESSAGES\gtk20.mo"
	SetOutPath "$INSTDIR\lib\locale\en@IPA\LC_MESSAGES\"
	File "Crack-Attack\lib\locale\en@IPA\LC_MESSAGES\gtk20.mo"
	SetOutPath "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-ani.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-bmp.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-gif.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-ico.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-jpeg.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-pcx.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-png.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-pnm.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-ras.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-tga.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-tiff.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-wbmp.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-xbm.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-xpm.dll"
	SetOutPath "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-am-et.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-cedilla.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-cyrillic-translit.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-ime.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-inuktitut.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-ipa.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-thai-broken.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-ti-er.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-ti-et.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\immodules\im-viqr.dll"
	SetOutPath "$INSTDIR\lib\gtk-2.0\2.4.0\engines\"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\engines\libbluecurve.dll"
	File "Crack-Attack\lib\gtk-2.0\2.4.0\engines\libwimp.dll"
	SetOutPath "$INSTDIR\localdata\"
	File "Crack-Attack\localdata\garbage_flavor_000.png"
	File "Crack-Attack\localdata\garbage_flavor_001.png"
	File "Crack-Attack\localdata\garbage_flavor_002.png"
	File "Crack-Attack\localdata\garbage_flavor_003.png"
	File "Crack-Attack\localdata\garbage_flavor_004.png"
	File "Crack-Attack\localdata\garbage_flavor_005.png"
	SetOutPath "$INSTDIR\share\themes\Default\gtk-2.0-key\"
	File "Crack-Attack\share\themes\Default\gtk-2.0-key\gtkrc"
	SetOutPath "$INSTDIR\share\themes\Default\gtk-2.0\"
	File "Crack-Attack\share\themes\Default\gtk-2.0\gtkrc.old"
	File "Crack-Attack\share\themes\Default\gtk-2.0\gtkrc.bluecurve"
	File "Crack-Attack\share\themes\Default\gtk-2.0\gtkrc"
	SetOutPath "$INSTDIR\share\themes\Emacs\gtk-2.0-key\"
	File "Crack-Attack\share\themes\Emacs\gtk-2.0-key\gtkrc"
	SetOutPath "$INSTDIR\share\man\man3\"
	File "Crack-Attack\share\man\man3\bind_textdomain_codeset.3"
	File "Crack-Attack\share\man\man3\bindtextdomain.3"
	File "Crack-Attack\share\man\man3\dcgettext.3"
	File "Crack-Attack\share\man\man3\dcngettext.3"
	File "Crack-Attack\share\man\man3\dgettext.3"
	File "Crack-Attack\share\man\man3\dngettext.3"
	File "Crack-Attack\share\man\man3\gettext.3"
	File "Crack-Attack\share\man\man3\iconv.3"
	File "Crack-Attack\share\man\man3\iconv_close.3"
	File "Crack-Attack\share\man\man3\iconv_open.3"
	File "Crack-Attack\share\man\man3\ngettext.3"
	File "Crack-Attack\share\man\man3\textdomain.3"
	SetOutPath "$INSTDIR\share\man\man1\"
	File "Crack-Attack\share\man\man1\envsubst.1"
	File "Crack-Attack\share\man\man1\gettext.1"
	File "Crack-Attack\share\man\man1\iconv.1"
	File "Crack-Attack\share\man\man1\ngettext.1"
	SetOutPath "$INSTDIR\share\locale\en@quot\LC_MESSAGES\"
	File "Crack-Attack\share\locale\en@quot\LC_MESSAGES\gettext-runtime.mo"
	SetOutPath "$INSTDIR\share\locale\en@boldquot\LC_MESSAGES\"
	File "Crack-Attack\share\locale\en@boldquot\LC_MESSAGES\gettext-runtime.mo"
	SetOutPath "$INSTDIR\bin"

	CreateDirectory "$SMPROGRAMS\Crack-Attack!"
	CreateShortCut "$SMPROGRAMS\Crack-Attack!\Crack-Attack!.lnk" "$INSTDIR\bin\crack-attack-gui.exe"
	CreateShortCut "$SMPROGRAMS\Crack-Attack!\Uninstall.lnk" "$INSTDIR\uninstall.exe"
	SetShellVarContext current ; install desktop icon just for current user
	CreateShortCut "$DESKTOP\Crack-Attack!.lnk" "$INSTDIR\bin\crack-attack-gui.exe"

SectionEnd

Section -FinishSection

	;Write Registry Keys
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"

	;Write Uninstaller
	WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section Uninstall

	;Remove from registry...
	DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"

	ClearErrors

	; Delete Shortcuts
	Delete "$DESKTOP\Crack-Attack!.lnk"
	Delete "$SMPROGRAMS\Crack-Attack!\Crack-Attack!.lnk"
	Delete "$SMPROGRAMS\Crack-Attack!\Uninstall.lnk"
	Delete "$SMPROGRAMS\Crack-Attack!\Website.lnk"

	IntCmp $isadmin 1 admin_uninst ahead_uninst

admin_uninst:
	SetShellVarContext all
	Delete "$DESKTOP\Crack-Attack!.lnk"
	Delete "$SMPROGRAMS\Crack-Attack!\Crack-Attack!.lnk"
	Delete "$SMPROGRAMS\Crack-Attack!\Uninstall.lnk"
	Delete "$SMPROGRAMS\Crack-Attack!\Website.lnk"
	SetShellVarContext current

ahead_uninst:

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Clean up Crack-Attack!
	Delete "$INSTDIR\COPYING.LIB-2"
	Delete "$INSTDIR\README.libiconv"
	Delete "$INSTDIR\README.txt"
	Delete "$INSTDIR\COPYING.txt"
	Delete "$INSTDIR\bin\gettext.sh"
	Delete "$INSTDIR\bin\crack-attack.exe"
	Delete "$INSTDIR\bin\crack-attack-gui.exe"
	Delete "$INSTDIR\bin\envsubst.exe"
	Delete "$INSTDIR\bin\gettext.exe"
	Delete "$INSTDIR\bin\iconv.exe"
	Delete "$INSTDIR\bin\ngettext.exe"
	Delete "$INSTDIR\bin\asprintf.dll"
	Delete "$INSTDIR\bin\charset.dll"
	Delete "$INSTDIR\bin\freetype6.dll"
	Delete "$INSTDIR\bin\gspawn-win32-helper.exe"
	Delete "$INSTDIR\bin\iconv.dll"
	Delete "$INSTDIR\bin\intl.dll"
	Delete "$INSTDIR\bin\jpeg62.dll"
	Delete "$INSTDIR\bin\libatk-1.0-0.dll"
	Delete "$INSTDIR\bin\libexpat-0.dll"
	Delete "$INSTDIR\bin\libfontconfig-1.dll"
	Delete "$INSTDIR\bin\libgdk-win32-2.0-0.dll"
	Delete "$INSTDIR\bin\libgdk_pixbuf-2.0-0.dll"
	Delete "$INSTDIR\bin\libglib-2.0-0.dll"
	Delete "$INSTDIR\bin\libgmodule-2.0-0.dll"
	Delete "$INSTDIR\bin\libgobject-2.0-0.dll"
	Delete "$INSTDIR\bin\libgthread-2.0-0.dll"
	Delete "$INSTDIR\bin\libgtk-win32-2.0-0.dll"
	Delete "$INSTDIR\bin\libpango-1.0-0.dll"
	Delete "$INSTDIR\bin\libpangoft2-1.0-0.dll"
	Delete "$INSTDIR\bin\libpangowin32-1.0-0.dll"
	Delete "$INSTDIR\bin\libpng12.dll"
	Delete "$INSTDIR\bin\libpng13.dll"
	Delete "$INSTDIR\bin\libtiff3.dll"
	Delete "$INSTDIR\bin\pango-querymodules.exe"
	Delete "$INSTDIR\bin\xmlparse.dll"
	Delete "$INSTDIR\bin\xmltok.dll"
	Delete "$INSTDIR\bin\zlib1.dll"
	Delete "$INSTDIR\data\default_multiplier"
	Delete "$INSTDIR\data\default_record"
	Delete "$INSTDIR\data\Makefile.am"
	Delete "$INSTDIR\data\clock_0.tga"
	Delete "$INSTDIR\data\clock_1.tga"
	Delete "$INSTDIR\data\clock_2.tga"
	Delete "$INSTDIR\data\clock_3.tga"
	Delete "$INSTDIR\data\clock_4.tga"
	Delete "$INSTDIR\data\clock_5.tga"
	Delete "$INSTDIR\data\clock_6.tga"
	Delete "$INSTDIR\data\clock_7.tga"
	Delete "$INSTDIR\data\clock_8.tga"
	Delete "$INSTDIR\data\clock_9.tga"
	Delete "$INSTDIR\data\clock_extra.tga"
	Delete "$INSTDIR\data\count_down_1.png"
	Delete "$INSTDIR\data\count_down_2.png"
	Delete "$INSTDIR\data\count_down_3.png"
	Delete "$INSTDIR\data\count_down_go.png"
	Delete "$INSTDIR\data\flavor_1.png"
	Delete "$INSTDIR\data\flavor_2.png"
	Delete "$INSTDIR\data\font0_0.tga"
	Delete "$INSTDIR\data\font0_1.tga"
	Delete "$INSTDIR\data\font0_2.tga"
	Delete "$INSTDIR\data\font0_3.tga"
	Delete "$INSTDIR\data\font0_4.tga"
	Delete "$INSTDIR\data\font0_5.tga"
	Delete "$INSTDIR\data\font0_6.tga"
	Delete "$INSTDIR\data\font0_7.tga"
	Delete "$INSTDIR\data\font0_8.tga"
	Delete "$INSTDIR\data\font0_9.tga"
	Delete "$INSTDIR\data\font0_a.tga"
	Delete "$INSTDIR\data\font0_and.tga"
	Delete "$INSTDIR\data\font0_at.tga"
	Delete "$INSTDIR\data\font0_b.tga"
	Delete "$INSTDIR\data\font0_br.tga"
	Delete "$INSTDIR\data\font0_c.tga"
	Delete "$INSTDIR\data\font0_ca.tga"
	Delete "$INSTDIR\data\font0_cb.tga"
	Delete "$INSTDIR\data\font0_cc.tga"
	Delete "$INSTDIR\data\font0_cd.tga"
	Delete "$INSTDIR\data\font0_ce.tga"
	Delete "$INSTDIR\data\font0_cf.tga"
	Delete "$INSTDIR\data\font0_cg.tga"
	Delete "$INSTDIR\data\font0_ch.tga"
	Delete "$INSTDIR\data\font0_ci.tga"
	Delete "$INSTDIR\data\font0_cj.tga"
	Delete "$INSTDIR\data\font0_ck.tga"
	Delete "$INSTDIR\data\font0_cl.tga"
	Delete "$INSTDIR\data\font0_cln.tga"
	Delete "$INSTDIR\data\font0_cm.tga"
	Delete "$INSTDIR\data\font0_cma.tga"
	Delete "$INSTDIR\data\font0_cn.tga"
	Delete "$INSTDIR\data\font0_co.tga"
	Delete "$INSTDIR\data\font0_cp.tga"
	Delete "$INSTDIR\data\font0_cq.tga"
	Delete "$INSTDIR\data\font0_cr.tga"
	Delete "$INSTDIR\data\font0_cs.tga"
	Delete "$INSTDIR\data\font0_ct.tga"
	Delete "$INSTDIR\data\font0_cu.tga"
	Delete "$INSTDIR\data\font0_cv.tga"
	Delete "$INSTDIR\data\font0_cw.tga"
	Delete "$INSTDIR\data\font0_cx.tga"
	Delete "$INSTDIR\data\font0_cy.tga"
	Delete "$INSTDIR\data\font0_cz.tga"
	Delete "$INSTDIR\data\font0_d.tga"
	Delete "$INSTDIR\data\font0_ds.tga"
	Delete "$INSTDIR\data\font0_e.tga"
	Delete "$INSTDIR\data\font0_ep.tga"
	Delete "$INSTDIR\data\font0_eq.tga"
	Delete "$INSTDIR\data\font0_f.tga"
	Delete "$INSTDIR\data\font0_g.tga"
	Delete "$INSTDIR\data\font0_gt.tga"
	Delete "$INSTDIR\data\font0_h.tga"
	Delete "$INSTDIR\data\font0_i.tga"
	Delete "$INSTDIR\data\font0_j.tga"
	Delete "$INSTDIR\data\font0_k.tga"
	Delete "$INSTDIR\data\font0_l.tga"
	Delete "$INSTDIR\data\font0_lt.tga"
	Delete "$INSTDIR\data\font0_m.tga"
	Delete "$INSTDIR\data\font0_mn.tga"
	Delete "$INSTDIR\data\font0_n.tga"
	Delete "$INSTDIR\data\font0_o.tga"
	Delete "$INSTDIR\data\font0_p.tga"
	Delete "$INSTDIR\data\font0_pd.tga"
	Delete "$INSTDIR\data\font0_pe.tga"
	Delete "$INSTDIR\data\font0_pl.tga"
	Delete "$INSTDIR\data\font0_pr.tga"
	Delete "$INSTDIR\data\font0_ps.tga"
	Delete "$INSTDIR\data\font0_pu.tga"
	Delete "$INSTDIR\data\font0_q.tga"
	Delete "$INSTDIR\data\font0_qm.tga"
	Delete "$INSTDIR\data\font0_r.tga"
	Delete "$INSTDIR\data\font0_s.tga"
	Delete "$INSTDIR\data\font0_sl.tga"
	Delete "$INSTDIR\data\font0_t.tga"
	Delete "$INSTDIR\data\font0_td.tga"
	Delete "$INSTDIR\data\font0_u.tga"
	Delete "$INSTDIR\data\font0_v.tga"
	Delete "$INSTDIR\data\font0_w.tga"
	Delete "$INSTDIR\data\font0_x.tga"
	Delete "$INSTDIR\data\font0_y.tga"
	Delete "$INSTDIR\data\font0_z.tga"
	Delete "$INSTDIR\data\font0_za.tga"
	Delete "$INSTDIR\data\font0_zb.tga"
	Delete "$INSTDIR\data\font0_zc.tga"
	Delete "$INSTDIR\data\font0_zd.tga"
	Delete "$INSTDIR\data\garbage_flavor_000.png"
	Delete "$INSTDIR\data\garbage_flavor_001.png"
	Delete "$INSTDIR\data\garbage_flavor_002.png"
	Delete "$INSTDIR\data\garbage_flavor_003.png"
	Delete "$INSTDIR\data\garbage_flavor_004.png"
	Delete "$INSTDIR\data\garbage_flavor_005.png"
	Delete "$INSTDIR\data\garbage_flavor_logo.png"
	Delete "$INSTDIR\data\garbage_flavor_logo_x.png"
	Delete "$INSTDIR\data\garbage_lightmap_0.png"
	Delete "$INSTDIR\data\garbage_lightmap_1.png"
	Delete "$INSTDIR\data\garbage_lightmap_2.png"
	Delete "$INSTDIR\data\garbage_lightmap_3.png"
	Delete "$INSTDIR\data\garbage_lightmap_4.png"
	Delete "$INSTDIR\data\garbage_lightmap_5.png"
	Delete "$INSTDIR\data\logo.png"
	Delete "$INSTDIR\data\logo_x.png"
	Delete "$INSTDIR\data\message_anykey.png"
	Delete "$INSTDIR\data\message_game_over.png"
	Delete "$INSTDIR\data\message_loser.png"
	Delete "$INSTDIR\data\message_paused.png"
	Delete "$INSTDIR\data\message_waiting.png"
	Delete "$INSTDIR\data\message_winner.png"
	Delete "$INSTDIR\data\sign_10.tga"
	Delete "$INSTDIR\data\sign_11.tga"
	Delete "$INSTDIR\data\sign_12.tga"
	Delete "$INSTDIR\data\sign_4.tga"
	Delete "$INSTDIR\data\sign_5.tga"
	Delete "$INSTDIR\data\sign_6.tga"
	Delete "$INSTDIR\data\sign_7.tga"
	Delete "$INSTDIR\data\sign_8.tga"
	Delete "$INSTDIR\data\sign_9.tga"
	Delete "$INSTDIR\data\sign_bonus.tga"
	Delete "$INSTDIR\data\sign_x10.tga"
	Delete "$INSTDIR\data\sign_x11.tga"
	Delete "$INSTDIR\data\sign_x12.tga"
	Delete "$INSTDIR\data\sign_x2.tga"
	Delete "$INSTDIR\data\sign_x3.tga"
	Delete "$INSTDIR\data\sign_x4.tga"
	Delete "$INSTDIR\data\sign_x5.tga"
	Delete "$INSTDIR\data\sign_x6.tga"
	Delete "$INSTDIR\data\sign_x7.tga"
	Delete "$INSTDIR\data\sign_x8.tga"
	Delete "$INSTDIR\data\sign_x9.tga"
	Delete "$INSTDIR\data\preview_reduced.png"
	Delete "$INSTDIR\data\preview_normal.png"
	Delete "$INSTDIR\data\preview_extremely_reduced.png"
	Delete "$INSTDIR\data\crack-attack.xpm"
	Delete "$INSTDIR\etc\pango\pango.aliases"
	Delete "$INSTDIR\etc\pango\pango.modules"
	Delete "$INSTDIR\etc\gtk-2.0\gdk-pixbuf.loaders"
	Delete "$INSTDIR\etc\gtk-2.0\gtk.immodules"
	Delete "$INSTDIR\etc\gtk-2.0\gtkrc"
	Delete "$INSTDIR\etc\fonts\fonts.conf"
	Delete "$INSTDIR\etc\fonts\local.conf"
	Delete "$INSTDIR\include\autosprintf.h"
	Delete "$INSTDIR\include\iconv.h"
	Delete "$INSTDIR\include\libcharset.h"
	Delete "$INSTDIR\include\libintl.h"
	Delete "$INSTDIR\include\localcharset.h"
	Delete "$INSTDIR\lib\libiconv.a"
	Delete "$INSTDIR\lib\libintl.a"
	Delete "$INSTDIR\lib\asprintf.lib"
	Delete "$INSTDIR\lib\charset.lib"
	Delete "$INSTDIR\lib\iconv.lib"
	Delete "$INSTDIR\lib\intl.lib"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-arabic-fc.dll"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-basic-fc.dll"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-basic-win32.dll"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-hangul-fc.dll"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-hebrew-fc.dll"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-indic-fc.dll"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-syriac-fc.dll"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-thai-fc.dll"
	Delete "$INSTDIR\lib\pango\1.4.0\modules\pango-tibetan-fc.dll"
	Delete "$INSTDIR\lib\locale\en_GB\LC_MESSAGES\atk10.mo"
	Delete "$INSTDIR\lib\locale\en_GB\LC_MESSAGES\glib20.mo"
	Delete "$INSTDIR\lib\locale\en_GB\LC_MESSAGES\gtk20.mo"
	Delete "$INSTDIR\lib\locale\en_CA\LC_MESSAGES\atk10.mo"
	Delete "$INSTDIR\lib\locale\en_CA\LC_MESSAGES\glib20.mo"
	Delete "$INSTDIR\lib\locale\en_CA\LC_MESSAGES\gtk20.mo"
	Delete "$INSTDIR\lib\locale\en@IPA\LC_MESSAGES\gtk20.mo"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-ani.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-bmp.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-gif.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-ico.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-jpeg.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-pcx.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-png.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-pnm.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-ras.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-tga.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-tiff.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-wbmp.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-xbm.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\libpixbufloader-xpm.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-am-et.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-cedilla.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-cyrillic-translit.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-ime.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-inuktitut.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-ipa.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-thai-broken.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-ti-er.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-ti-et.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\im-viqr.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\engines\libbluecurve.dll"
	Delete "$INSTDIR\lib\gtk-2.0\2.4.0\engines\libwimp.dll"
	Delete "$INSTDIR\localdata\garbage_flavor_000.png"
	Delete "$INSTDIR\localdata\garbage_flavor_001.png"
	Delete "$INSTDIR\localdata\garbage_flavor_002.png"
	Delete "$INSTDIR\localdata\garbage_flavor_003.png"
	Delete "$INSTDIR\localdata\garbage_flavor_004.png"
	Delete "$INSTDIR\localdata\garbage_flavor_005.png"
	Delete "$INSTDIR\share\themes\Default\gtk-2.0-key\gtkrc"
	Delete "$INSTDIR\share\themes\Default\gtk-2.0\gtkrc.old"
	Delete "$INSTDIR\share\themes\Default\gtk-2.0\gtkrc.bluecurve"
	Delete "$INSTDIR\share\themes\Default\gtk-2.0\gtkrc"
	Delete "$INSTDIR\share\themes\Emacs\gtk-2.0-key\gtkrc"
	Delete "$INSTDIR\share\man\man3\bind_textdomain_codeset.3"
	Delete "$INSTDIR\share\man\man3\bindtextdomain.3"
	Delete "$INSTDIR\share\man\man3\dcgettext.3"
	Delete "$INSTDIR\share\man\man3\dcngettext.3"
	Delete "$INSTDIR\share\man\man3\dgettext.3"
	Delete "$INSTDIR\share\man\man3\dngettext.3"
	Delete "$INSTDIR\share\man\man3\gettext.3"
	Delete "$INSTDIR\share\man\man3\iconv.3"
	Delete "$INSTDIR\share\man\man3\iconv_close.3"
	Delete "$INSTDIR\share\man\man3\iconv_open.3"
	Delete "$INSTDIR\share\man\man3\ngettext.3"
	Delete "$INSTDIR\share\man\man3\textdomain.3"
	Delete "$INSTDIR\share\man\man1\envsubst.1"
	Delete "$INSTDIR\share\man\man1\gettext.1"
	Delete "$INSTDIR\share\man\man1\iconv.1"
	Delete "$INSTDIR\share\man\man1\ngettext.1"
	Delete "$INSTDIR\share\locale\en@quot\LC_MESSAGES\gettext-runtime.mo"
	Delete "$INSTDIR\share\locale\en@boldquot\LC_MESSAGES\gettext-runtime.mo"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\Crack-Attack!"
	RMDir "$INSTDIR\share\themes\Emacs\gtk-2.0-key\"
	RMDir "$INSTDIR\share\themes\Emacs\"
	RMDir "$INSTDIR\share\themes\Default\gtk-2.0\"
	RMDir "$INSTDIR\share\themes\Default\gtk-2.0-key\"
	RMDir "$INSTDIR\share\themes\Default\"
	RMDir "$INSTDIR\share\themes\"
	RMDir "$INSTDIR\share\man\man3\"
	RMDir "$INSTDIR\share\man\man1\"
	RMDir "$INSTDIR\share\man\"
	RMDir "$INSTDIR\share\locale\en@quot\LC_MESSAGES\"
	RMDir "$INSTDIR\share\locale\en@quot\"
	RMDir "$INSTDIR\share\locale\en@boldquot\LC_MESSAGES\"
	RMDir "$INSTDIR\share\locale\en@boldquot\"
	RMDir "$INSTDIR\share\locale\"
	RMDir "$INSTDIR\share\"
	RMDir "$INSTDIR\lib\pango\1.4.0\modules\"
	RMDir "$INSTDIR\lib\pango\1.4.0\"
	RMDir "$INSTDIR\lib\pango\"
	RMDir "$INSTDIR\lib\locale\en_GB\LC_MESSAGES\"
	RMDir "$INSTDIR\lib\locale\en_GB\"
	RMDir "$INSTDIR\lib\locale\en_CA\LC_MESSAGES\"
	RMDir "$INSTDIR\lib\locale\en_CA\"
	RMDir "$INSTDIR\lib\locale\en@IPA\LC_MESSAGES\"
	RMDir "$INSTDIR\lib\locale\en@IPA\"
	RMDir "$INSTDIR\lib\locale\"
	RMDir "$INSTDIR\lib\gtk-2.0\2.4.0\loaders\"
	RMDir "$INSTDIR\lib\gtk-2.0\2.4.0\immodules\"
	RMDir "$INSTDIR\lib\gtk-2.0\2.4.0\engines\"
	RMDir "$INSTDIR\lib\gtk-2.0\2.4.0\"
	RMDir "$INSTDIR\lib\gtk-2.0\"
	RMDir "$INSTDIR\lib\"
	RMDir "$INSTDIR\include\"
	RMDir "$INSTDIR\etc\pango\"
	RMDir "$INSTDIR\etc\gtk-2.0\"
	RMDir "$INSTDIR\etc\fonts\"
	RMDir "$INSTDIR\etc\"
	RMDir "$INSTDIR\data\"
	RMDir "$INSTDIR\bin\"

	IfErrors part_success
	goto end

part_success:
	MessageBox MB_OK "Not all files could be deleted."

end:

SectionEnd

; eof
