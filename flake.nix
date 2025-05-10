{
  description = "A very basic flake";

  inputs = { nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable"; };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {

      devShells.${system}.default = with pkgs;
        mkShell {
          stdenv = llvmPackages_17.stdenv;
          buildInputs = [ llvmPackages_17.clang-tools cmake ];
        };
    };
}
