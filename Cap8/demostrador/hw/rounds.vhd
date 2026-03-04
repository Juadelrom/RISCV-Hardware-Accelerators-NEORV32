-- ====================================================================================
-- Archivo            : rounds.vhd
-- Autor original     : P. N. Vamshi (GitHub: pnvamshi)
-- Repositorio        : Hardware-Implementation-of-AES-VHDL
-- Fecha de creación  : Julio 2015
--
-- Nota de uso (TFG)  : Módulo de hardware original reutilizado como componente 
--                      combinacional para la nueva arquitectura iterativa de este 
--                      Trabajo Fin de Grado.
-- ====================================================================================

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity rounds is
    Port ( roundin : in  STD_LOGIC_VECTOR (127 downto 0);
           keyin : in  STD_LOGIC_VECTOR (127 downto 0);
           keyout : out  STD_LOGIC_VECTOR (127 downto 0);
           garbage : in STD_LOGIC_VECTOR (7 DOWNTO 0);
           roundout : out  STD_LOGIC_VECTOR (127 downto 0)
           );
end rounds;

architecture Behavioral of rounds is
component addroundkey is
    Port ( k_in : in std_logic_vector (127 downto 0);
           m_in : in  std_logic_vector (127 downto 0);
          add_out : out std_logic_vector (127 downto 0));
end component;

component subbytes is
    Port ( p_in : in  STD_LOGIC_VECTOR (127 downto 0);
           s_out : out  STD_LOGIC_VECTOR (127 downto 0));
end component;

component shiftrows is
    Port ( a : in  STD_LOGIC_VECTOR (127 downto 0);
           c : out  STD_LOGIC_VECTOR (127 downto 0));
end component;

component mixcolumns is
    Port ( a : in  STD_LOGIC_VECTOR (127 downto 0);
           mixout : out  STD_LOGIC_VECTOR (127 downto 0));
end component;

component KeyGenerator is
Port ( a  :in  STD_LOGIC_VECTOR (127 downto 0);
           rcon  :  in  STD_LOGIC_VECTOR (7 downto 0);
           b  :  out  STD_LOGIC_VECTOR (127 downto 0));
end component;

signal subout,shiftout,mixcolout:std_logic_vector(127 downto 0);
signal keyout_i: STD_LOGIC_VECTOR (127 downto 0);

begin

w1: KeyGenerator port map (keyin,garbage,keyout_i);

w2:subbytes port map(roundin,subout);

w3:shiftrows  port map(subout,shiftout);

w4:mixcolumns port map(shiftout,mixcolout);

w5:addroundkey  port map(keyout_i,mixcolout,roundout);

keyout <= keyout_i;

end Behavioral;

