{
  description = "C/C++ development environment with CMake and LSP support";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      forAllSystems =
        function:
        nixpkgs.lib.genAttrs systems (
          system:
            function (
              import nixpkgs {
                inherit system;
              }
            ) system
        );
    in
    {
      packages = forAllSystems (pkgs: _system: {
        default = pkgs.stdenv.mkDerivation {
          pname = "gambling-gladiator";
          version = "0.1.0";

          src = ./.;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
          ];

          buildInputs = with pkgs; [
            enet
            glfw
            raylib
          ];

          cmakeFlags = [
            "-G Ninja"
            "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
          ];

          meta.mainProgram = "gambling-gladiator";
        };
      });

      apps = forAllSystems (pkgs: system: {
        default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/gambling-gladiator";
          meta.description = "Run Gambling Gladiator";
        };
      });

      devShells = forAllSystems (pkgs: _system: {
        default = pkgs.mkShell {
          packages = with pkgs; [
            cmake
            ninja
            pkg-config

            enet
            glfw
            raylib

            clang-tools
            gdb
            lldb

            gcc
          ];

          shellHook = ''
            export CMAKE_GENERATOR=Ninja
            echo "C/C++ dev shell ready."
            echo "Configure: cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
            echo "Build:     cmake --build build"
            echo "LSP:       clangd will use build/compile_commands.json"
          '';
        };
      });

      formatter = forAllSystems (pkgs: _system: pkgs.nixfmt);
    };
}
