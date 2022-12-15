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
        nixConfig.sandbox = "relaxed";
        devShell = pkgs.devshell.mkShell {
          name = "eenum";
          bash = {
            extra = ''
              meson build > /dev/null
            '';
            interactive = "";
          };
          commands = [
            {
              name = "build";
              help = "Build the project and run tests";
              command = "ninja -C build test";
            }
          ];
          env = [
          ];
          packages =
            with pkgs;
            [
              clang
              cmake
              ninja
              meson
            ];
        };
      });
}
