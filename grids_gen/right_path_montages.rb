## This script takes a .dat DAG file representing a Montage
## application and generates 7 new ones, each one with an additional
## task on the rightmost path dependant on the VM 14.

#!/usr/bin/env ruby

if ARGV.size != 1
  puts "Usage #{$0} Leve2_width"
end

def gen_montage_children(n)
  larger_lvls = [0, n, 3*n - 1, 5*n - 2, 5*n - 1, 5*n, 6*n].reverse
  name = "montage" + n.to_s + "_app.dat"
  origin = File.new(name, "r")
  all = origin.readlines
  origin.close
  s, rest = all[2].split(/\) /)
  s += ") "
  rest.delete!("\]")
  vms = rest.split(/ /).map {|i| i.to_i}
  n_changed = 1
  larger_lvls.each do |v|
    vms[v] = 14
    name = "montage" + n.to_s + "_child_" + n_changed.to_s + ".dat"
    n_changed += 1
    child = File.new(name, "w")
    child.puts(all[0])
    child.puts(all[1])
    child.print(s)
    (0...vms.size).each {|e| child.print vms[e].to_s + " "}
    child.puts("]")
    (3...all.size).each {|e| child.puts all[e]}
    child.close
  end

end

gen_montage_children(Integer(ARGV[0]))
