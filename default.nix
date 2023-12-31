{ pkgs ? import <nixpkgs> {} }:

  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = with pkgs.buildPackages; [ 
      cmake
      libGL
      xorg.libXrandr 
      xorg.libXinerama
      xorg.libXcursor
      xorg.libXi
      xorg.libXxf86vm
    ];

    shellHook = ''
      alias build="cmake --build build/"
      alias run="./build/ParticleSimulation"
    '';

}