%define  ver     1.1.0
%define  rel     1
%define  prefix  /usr

Summary: Dr. Genius advanced math tool
Name: drgenius
Version: %ver
Release: %rel
Copyright: GPL
Group: X11/Applications
Source: drgenius-%{ver}.tar.gz
Url: http://www.gnome.org/
BuildRoot: /var/tmp/drgenius-%{ver}-root
Prefix: %prefix

Requires: gtk+ >= 1.2.0
Requires: gnome-libs >= 1.0.54
Requires: libglade >= 0.11
Requires: libxml >= 1.7.3
Requires: gob


%description 
Dr. Genius is  an euclidian geometry tool.

%changelog
* Wed Feb 28 2001 Hilaire Fernandes <hilaire@ext.cri74.org>
- remove the genius stuff

* Wed Jun 21 2000 Hilaire Fernandes <hilaire@seul.org>
- updated the %description, %file and others outdated fields.

* Fri Nov 19 1999 George Lebl <jirka@5z.com>
- updated for Dr.Genius from Genius

* Tue Apr 20 1999 Erik Walthinsen <omega@cse.ogi.edu>
- added spec.in file, changes to makefiles as approrpiate

%prep
%setup

%build
./configure --prefix=%prefix

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
rm -rf $RPM_BUILD_ROOT

make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)

%doc README ABOUT-NLS AUTHORS COPYING NEWS TODO ChangeLog

%{prefix}/bin/*
%{prefix}/share/locale/*/*/*
%{prefix}/share/drgenius/*
%{prefix}/share/gnome/apps/Utilities/drgenius.desktop
%{prefix}/share/pixmaps/drgenius.png
%{prefix}/share/pixmaps/gnome-drgenius-geo.png
%{prefix}/share/pixmaps/gnome-drgenius.png
%{prefix}/share/gnome/help/*
