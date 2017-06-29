Summary: General UNIX wrapper tool.
Name: toolwrap
Version: 1.0.0
Release: 1
Copyright: GPL
Group: Development/Tools
Source: toolwrap-1.0.0.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot

%description

Toolwrap is a wrapper tool (a program which starts other programs) which 
may  be used to launch applications according to defined preferences 
(or policies). Through its use, many releases of a given application 
can coexist, and may be deployed on a single workstation or made available
on a fileserver. Policy files allow different users to use different versions 
transparently. Environment  files control which binaries to use and prepare 
the environment before starting the application.

%prep
%setup -q

#
# /opt is always a good candidate to install toolwrap 
#
./configure --prefix=/opt

%build
make 

%install

# do not run 'make install here, as prefix is set to /opt'

rm -rf $RPM_BUILD_ROOT/opt/
mkdir -p $RPM_BUILD_ROOT/opt/pkgs/toolwrap-0.2/man/man1
mkdir -p $RPM_BUILD_ROOT/opt/etc
mkdir -p $RPM_BUILD_ROOT/opt/bin
mkdir -p $RPM_BUILD_ROOT/opt/env

install src/toolwrap $RPM_BUILD_ROOT/opt/bin
install src/toolwrap.1 $RPM_BUILD_ROOT/opt/pkgs/toolwrap-0.2/man/man1
install src/toolwrap-policies $RPM_BUILD_ROOT/opt/etc
install src/env.default $RPM_BUILD_ROOT/opt/env/__default__


%clean

%files
%doc README TODO COPYING doc/ref.sgml

/opt/bin/toolwrap

/opt/pkgs/toolwrap-0.2
%config /opt/env/__default__
%config /opt/etc/toolwrap-policies
#/opt/pkgs/toolwrap-0.2/man/man1/toolwrap.1





%changelog


* Tue Mar 29 2005 BM
- provide basename() fallback.
- fix some #ifdef's
- fix manual page installation prefix
- fix command line options parsing (POSIXILY_CORRECT behaviour)
- fix install arguments (remove -C & -D switches)
- should compiles cleanly on Linux (x86, amd64, alpha), *BSD, Solaris, OS X

* Wed Mar 16 2005  BM
- Initial release


