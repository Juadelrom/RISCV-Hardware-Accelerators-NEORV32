-- ====================================================================================
-- Módulo             : aes128_10_fsm.vhd
-- Autor              : Carlos Jesús Jiménez Fernández (Tutor)
-- Institución        : Universidad de Sevilla (US)
-- Departamento       : Departamento de Tecnología Electrónica
-- Centro de Invest.  : Instituto de Microelectrónica de Sevilla (IMSE-CNM)
-- ====================================================================================
-- Descripción de la Arquitectura:
-- Este módulo actúa como la entidad superior (top entity) del cifrador AES-128,
-- sustituyendo al archivo original "aes.vhd" del repositorio de referencia.
--
-- Problema del diseño original:
-- La implementación previa instanciaba las 10 rondas del estándar AES de forma 
-- puramente combinacional (completamente desenrollada). Esto generaba un camino 
-- crítico (critical path) inmanejable para la síntesis física, limitando severamente 
-- la frecuencia máxima de operación (fmax) y provocando un consumo de área masivo.
--
-- Solución implementada:
-- Se ha rediseñado el control mediante una Máquina de Estados Finita (FSM). 
-- Ahora, el cálculo se realiza de forma secuencial, reutilizando una única 
-- instancia del combinacional de ronda en cada ciclo de reloj.
-- ================================================================================
-- ================================================================================
-- MODIFICACIONES REALIZADAS PARA TRABAJO FIN DE GRADO (TFG):
-- ================================================================================
-- Archivo modificado: aes128_10_fsm.vhd
-- Autor modificación: Juan Manuel Delgado Romero
-- Fecha:              Febrero 2026
-- 
-- Institución:        Universidad de Sevilla - Escuela Politécnica Superior
-- Titulación:         Grado en Ingeniería Electrónica Industrial
-- 
-- Descripción de los cambios:
-- Se han modificado los estados inicial y final de la FSM de control para cumplir 
-- el protocolo de comunicación HW/SW definido en el Capítulo 5 de la memoria de 
-- este proyecto.
-- ================================================================================


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

entity AES128_10_fsm is
Port ( clk       :  in  STD_LOGIC;
       reset     :  in  std_logic;
       plaintext :  in  STD_LOGIC_VECTOR (127 downto 0);
       key       :  in  STD_LOGIC_VECTOR (127 downto 0);
       init:        in  std_logic;
       busy:        out std_logic;
       ciphertext : out STD_LOGIC_VECTOR (127 downto 0) 
      );
end AES128_10_fsm;

architecture Behavioral of AES128_10_fsm is

component rounds is
Port (     roundin  : in  STD_LOGIC_VECTOR (127 downto 0);
           keyin    : in  STD_LOGIC_VECTOR (127 downto 0);
           keyout   : out  STD_LOGIC_VECTOR (127 downto 0);
           garbage  : in std_logic_vector (7 downto 0);
           roundout : out  STD_LOGIC_VECTOR (127 downto 0));
end component;

component roundlast is
Port ( roundlastin  : in  STD_LOGIC_VECTOR (127 downto 0);
       keylastin    : in  STD_LOGIC_VECTOR (127 downto 0);
       garbage      : in std_logic_vector (7 downto 0);
       roundlastout : out  STD_LOGIC_VECTOR (127 downto 0));
end component;

signal dato_i, key_i: std_logic_vector(127 downto 0);
signal dato_o, key_o, dato_o_last: std_logic_vector(127 downto 0);

signal garbaje: std_logic_vector(7 downto 0);

-- Señales para la FSM
   type estate_type is (espera, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, data_out);
   signal estado: estate_type;

begin

   uut_rounds: rounds PORT MAP(
      roundin => dato_i,
      keyin => key_i,
      keyout => key_o,
      garbage => garbaje,
      roundout => dato_o
   );

   uut_roundlast: roundlast PORT MAP(
      roundlastin => dato_i,
      keylastin   => key_i,
      garbage     => garbaje,
      roundlastout => dato_o_last
   );

   P_FSM:process(clk, reset)
   begin
      if reset = '1' then
         estado <= espera;
         busy <= '0';
         garbaje <= x"00";
         
      elsif rising_edge(clk) then
         --done <= '0';
         case estado is
            when espera =>
               busy <= '0';
               if init = '1' then
                  busy <= '1';
                  dato_i <= plaintext;
                  key_i  <= key;
                  estado <= r0;
                  --cont <= (others => '0');
               end if;
               garbaje <= x"00";
            
            when r0 => -- preround
                  dato_i <= dato_i XOR key_i;
                  estado <= r1;
                 garbaje <= x"01";
                  --cont <= "0001";
            
            when r1 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"02";
                 estado  <= r2;

            when r2 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"04";
                 estado  <= r3;

            when r3 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"08";
                 estado  <= r4;

            when r4 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"10";
                 estado  <= r5;

            when r5 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"20";
                 estado  <= r6;

            when r6 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"40";
                 estado  <= r7;

            when r7 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"80";
                 estado  <= r8;

            when r8 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"1B";
                 estado  <= r9;

            when r9 =>
                 dato_i  <= dato_o;
                 key_i   <= key_o;
                 garbaje <= x"36";
                 estado  <= r10;

            when r10 =>
                 estado <= data_out;
                 dato_i <= dato_o_last;
                 garbaje <= x"00";
                           
            when data_out =>
               ciphertext <= dato_i;
               busy <= '0';
               garbaje <= x"00";
               if init = '0' then 
               estado <= espera;
               end if;
            
         end case;
      end if;
   end process;

end Behavioral;
