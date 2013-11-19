using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace RylPacketAnalyzer
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public App()
        {
            RylPacketAnalyzer.Entities ent = new Entities();

            Guid id = Guid.NewGuid();

            var t = ent.TesteRel.Select(m=> new{ m.Id, m.Name, m.Id_Teste, m.Teste.Nome });

            t.First().

            
            
            
            Action<string> m = texto => Console.Write(texto);            
        }
    }
}
