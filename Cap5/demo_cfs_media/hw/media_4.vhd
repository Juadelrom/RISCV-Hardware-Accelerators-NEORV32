--------------------------------------------------------------------------------
-- Archivo:     media_4.vhd
-- Autor:       Juan Manuel Delgado Romero
-- Fecha:       Febrero 2026
--
-- Institución: Universidad de Sevilla - Escuela Politécnica Superior
-- Titulación:  Grado en Ingeniería Electrónica Industrial
-- Proyecto:    Trabajo Fin de Grado (TFG)
-- Licencia:    MIT (Ver archivo LICENSE)
--
-- Descripción: 
-- Calcula la media entera sin signo de 4 enteros de 32 bits.
--------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

library neorv32;
use neorv32.neorv32_package.all;


entity media_4 is
  Port ( a,b,c,d: in  std_ulogic_vector(31 downto 0);
         media  : out std_ulogic_vector(31 downto 0)
        );
end media_4;

architecture Behavioral of media_4 is
  signal suma: unsigned(31 downto 0);
begin
  suma  <= unsigned(a)+ unsigned(b) + unsigned(c) + unsigned(d);
  media <= std_ulogic_vector(suma srl 2);

end Behavioral;
