## Creates a .dot DAG file based on a DAG input.
## Usage example:
## ./dag2dot.rb /path/to/.dag/file [/path/to/output/file]

#!/usr/bin/env ruby

if ARGV.length == 0
  puts "Usage: #{$0} /path/to/dag/file"
  Process.exit
end

output = ARGV.length == 1 ? $> : File.new(ARGV[1], "w")

if ARGV.length == 2 && ARGV[1].include?(".")
  output.puts "digraph #{ARGV[1].split(".").first} {"
else
  output.puts "digraph outGraph {"
end

row = 0

File.open(ARGV[0]) do |input|
  input.each_line do |line|
    line.split.each.with_index do |num, i|
      if Integer(num) > 0
        output.puts "    #{row} -> #{i};"
      end
    end
    row += 1
  end
end

output.puts "}"
