----------------------------------------------------------------------------------
-- Company: DTE-US
-- Engineer: CJJF
-- 
-- Create Date:  
-- Design Name: 
-- Module Name:    divisor_32 - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: Divisor de dos números positivos de 32 bits
--              Utilizando el algoritmo de resta sin restauracion
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

entity divisor_32 is
    Port ( clk : in  STD_LOGIC;
           reset : in  STD_LOGIC;
           init : in  STD_LOGIC;
           dividendo : in  STD_LOGIC_VECTOR (31 downto 0);
           divisor : in  STD_LOGIC_VECTOR (31 downto 0);
           cociente : out  STD_LOGIC_VECTOR (31 downto 0);
           resto : out  STD_LOGIC_VECTOR (31 downto 0);
           --done : out  STD_LOGIC;
           error : out  STD_LOGIC;
           busy : out STD_LOGIC);
end divisor_32;   


architecture Behavioral of divisor_32 is

   constant nb: integer := 31;

   signal q: std_logic_vector(nb downto 0);
   signal m: signed(nb+1 downto 0);
   signal a: signed(nb+1 downto 0);
   signal n: unsigned(4 downto 0);

   type state_type is (start, calc, calc2, restore, end_p);
   signal state: state_type;

begin

   P1:process(clk, reset)
   begin
      if reset = '1' then
         state <= start;
         --done <= '0';
         error <= '0';
         busy  <= '0';
      elsif rising_edge(clk) then
         case state is
            when start =>
				   --done <= '0';
               if init = '1' and unsigned(divisor) /= 0 then
                  m <= '0' & signed(divisor);
                  a <= (others => '0');
                  q <= dividendo;
                  n <= to_unsigned(nb, 5);
                  --done <= '0';
                  error <= '0';
                  state <= calc;
                  busy  <= '1';
               elsif init = '1' and unsigned(divisor) = 0 then
                  error <= '1';
               end if;
            
            when calc =>
               q <= q(nb-1 downto 0) & '0';
               if a(nb+1) = '1' then
                  a <= (a(nb downto 0) & q(nb)) + m;
               else
                  a <= (a(nb downto 0) & q(nb)) - m;
               end if;
               state <= calc2;
            
            when calc2 =>
               q(0) <= not a(nb + 1);
               if n > 0 then
                  state <= calc;
                  n <= n - 1;
               else
                  state <= restore;
               end if;
            
            when restore =>
               if a(nb+1) = '1' then
                  a <= a + m;
               end if;
               state <= end_p;
            
            when end_p =>
               cociente <= std_logic_vector(q);
               resto   <= std_logic_vector(a(nb downto 0));
               --done <= '1';
               --state <= start;
               busy <= '0';
               if init = '0' then
                state <= start;
               end if;

         end case;     

      end if;
   end process;


end Behavioral;