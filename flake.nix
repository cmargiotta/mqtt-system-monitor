{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils/master";
    devshell.url = "github:numtide/devshell/master";
  };
  outputs = { self, nixpkgs, flake-utils, devshell }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ devshell.overlay ];
          config.allowUnfree = true;
        };
      in
      rec {
        devShell = pkgs.devshell.mkShell {
          name = "msm";
          bash = {
            interactive = "";
          };
          commands = [
            {
              name = "build";
              help = "Build the project and run tests";
              command = "cargo build";
            }
          ];
          env = [
          ];
          packages = with pkgs; [
            cargo
            rustc
          ];
        };
      });
}
